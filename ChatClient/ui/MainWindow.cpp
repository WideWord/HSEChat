#include "MainWindow.hpp"
#include "LoginDialog.hpp"
#include <QUrl>

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


    connect(&loginDialog, &LoginDialog::rejected, [this]() {
        close();
    });

    connect(&loginDialog, &LoginDialog::loginDataEntered, [this](const QString& host, const QString& username, const QString& password, bool signup) {
        statusLabel->setText("Подключение...");

        loginMessage.username = username;
        loginMessage.password = password;
        loginMessage.signUp = signup;

        int i = host.indexOf(':');
        int port = 8000;
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

        loginDialog.show();
    });

    connection->setIncomingMessageHandler<LoginResultMessage>([this](LoginResultMessage result) {
        if (result == LoginResultMessage::Ok) {
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
}

void MainWindow::showEvent(QShowEvent *ev) {
    QMainWindow::showEvent(ev);

    loginDialog.show();
}
