#include "MainWindow.hpp"
#include "LoginDialog.hpp"
#include <QUrl>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

    socket = new QTcpSocket();
    connection = new Connection(socket, this);

    statusLabel = new QLabel(this);
    statusBar()->addPermanentWidget(statusLabel);

    statusLabel->setText("Не подключён");
    setEnabled(false);

    connect(ui.exitButton, &QPushButton::clicked, [this]() {
        socket->disconnectFromHost();
    });

    connect(ui.addContactButton, &QPushButton::clicked, [this]() {
        QInputDialog* dialog = new QInputDialog(this);
        dialog->setLabelText("Введите имя контакта:");
        dialog->setModal(true);
        dialog->setCancelButtonText("Отменить");
        dialog->setOkButtonText("Добавить");
        connect(dialog, &QInputDialog::accepted, [this, dialog]() {
            AddContactMessage msg;
            msg.contactUsername = dialog->textValue();
            connection->sendMessage(msg);
        });
        dialog->setAttribute(Qt::WA_DeleteOnClose, true);
        dialog->show();
    });

    connect(ui.deleteContactButton, &QPushButton::clicked, [this]() {
        if (ui.usersListWidget->selectedItems().length() == 0) {
            return;
        }
        QString contact = ui.usersListWidget->selectedItems().first()->text();
        QMessageBox* box = new QMessageBox(QMessageBox::Question, "Удалить контакт", "Вы уверены что хотите удалить контакт " + contact + "?",
                                           QMessageBox::No | QMessageBox::Yes);
        box->setModal(true);
        box->setAttribute(Qt::WA_DeleteOnClose, true);

        connect(box, &QMessageBox::finished, [this, contact](int result) {
            if (result != QMessageBox::Yes) {
                return;
            }
            RemoveContactMessage msg;
            msg.contactUsername = contact;
            connection->sendMessage(msg);
        });

        box->show();
    });

    connection->setIncomingMessageHandler<AddContactResultMessage>([this](const AddContactResultMessage& msg) {
        QMessageBox* box;
        if (msg.ok) {
            box = new QMessageBox(QMessageBox::Information, "", "Контакт " + msg.contactUsername + " добавлен");
        } else {
            box = new QMessageBox(QMessageBox::Critical, "", "Не удалось добавить контакт " + msg.contactUsername);
        }
        box->setAttribute(Qt::WA_DeleteOnClose, true);
        box->setModal(true);
        box->show();
    });

    connect(&loginDialog, &LoginDialog::rejected, [this]() {
        close();
    });

    connect(&loginDialog, &LoginDialog::loginDataEntered, [this](const QString& host, const QString& username, const QString& password, bool signup) {
        statusLabel->setText("Подключение...");

        loginMessage.username = username;
        loginMessage.password = password;
        loginMessage.signUp = signup;

        int i = host.indexOf(':');
        int port = 8003;
        QString h = host;
        if (i > 0) {
            port = host.mid(i + 1, host.length() - (i + 1)).toInt();
            h = host.left(i);
        }

        socket->connectToHost(h, port);
    });

    connect(socket, &QTcpSocket::connected, [this]() {
        statusLabel->setText("Авторизация...");

        connection->sendMessage(loginMessage);
    });

    connect(socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), [this](QAbstractSocket::SocketError err) {
        statusLabel->setText("Ошибка подключения");
        setEnabled(false);

        loginDialog.show();
    });

    connect(socket, &QTcpSocket::disconnected, [this]() {
        setEnabled(false);
        statusLabel->setText("Соединение потеряно");
        resetState();
        loginDialog.show();
    });

    connection->setIncomingMessageHandler<LoginResultMessage>([this](LoginResultMessage result) {
        if (result.ok) {
            setEnabled(true);
            statusLabel->setText("Подключён");
        } else {
            socket->disconnectFromHost();

            if (loginMessage.signUp) {
                statusLabel->setText("Регистрация не удалась");
            } else {
                statusLabel->setText("Авторизация не удалась");
            }
            loginDialog.show();
        }
    });

    connection->setIncomingMessageHandler<ContactListMessage>([this](const ContactListMessage& msg) {
        ui.usersListWidget->clear();
        ui.usersListWidget->addItems(msg.users);
    });

    connection->setIncomingMessageHandler<MessagesMessage>([this](const MessagesMessage& msg) {
        QString cache;
        for (MessagesMessage::Message chatMsg : msg.messages) {
            cache += chatMsg.date.toString("yyyy.MM.dd hh:mm") + " " + (chatMsg.isMyMessage?loginMessage.username:msg.contactUsername);
            cache += ": " + chatMsg.body + "\n";
        }
        messagesCache[msg.contactUsername] = cache;
        if (ui.usersListWidget->selectedItems().length() != 0) {
            if (ui.usersListWidget->selectedItems()[0]->text() == msg.contactUsername) {
                ui.messagesTextView->setPlainText(cache);
            }
        }
    });

    connection->setIncomingMessageHandler<NewMessageMessage>([this](const NewMessageMessage& msg) {
        QString contactUsername;
        if (msg.sender == loginMessage.username) {
            contactUsername = msg.receiver;
        } else {
            contactUsername = msg.sender;
        }

        if (messagesCache.contains(contactUsername)) {
            messagesCache[contactUsername].append(msg.date.toString("yyyy.MM.dd hh:mm") + " " + msg.sender + ": " + msg.body + "\n");
            if (ui.usersListWidget->selectedItems().length() != 0) {
                if (ui.usersListWidget->selectedItems()[0]->text() == contactUsername) {
                    ui.messagesTextView->setPlainText(messagesCache[contactUsername]);
                }
            }
        }

    });

    connect(ui.usersListWidget, &QListWidget::itemSelectionChanged, [this]() {
        if (ui.usersListWidget->selectedItems().length() != 0) {
            QString contact = ui.usersListWidget->selectedItems()[0]->text();
            if (messagesCache.contains(contact)) {
                ui.messagesTextView->setPlainText(messagesCache[contact]);
            } else {
                ui.messagesTextView->setPlainText("Загрузка истории...");
                MessagesRequestMessage msg;
                msg.contactUsername = contact;
                connection->sendMessage(msg);
            }
        }
    });

    connect(ui.messageField, &QLineEdit::returnPressed, [this]() {
        if (ui.messageField->text().length() == 0) {
            return;
        }
        if (ui.usersListWidget->selectedItems().length() != 0) {
            QString contact = ui.usersListWidget->selectedItems()[0]->text();
            NewMessageMessage msg;
            msg.receiver = contact;
            msg.body = ui.messageField->text();
            connection->sendMessage(msg);
            ui.messageField->setText("");
        }
    });

    connect(qApp, &QApplication::aboutToQuit, [this]() {
        socket->disconnectFromHost();
    });
}

void MainWindow::resetState() {
    messagesCache.clear();
    ui.usersListWidget->clear();
    ui.messagesTextView->setPlainText("");
    ui.messageField->setText("");
}

void MainWindow::showEvent(QShowEvent *ev) {
    QMainWindow::showEvent(ev);

    loginDialog.show();
}
