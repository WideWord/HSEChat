#include "LoginDialog.hpp"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    setFixedSize(sizeHint());

    connect(ui.loginButton, &QPushButton::clicked, [this]() {
        accept();
        emit loginDataEntered(ui.hostField->text(), ui.usernameField->text(), ui.passwordField->text(), false);
        ui.passwordField->setText("");
    });

    connect(ui.signupButton, &QPushButton::clicked, [this]() {
        accept();
        emit loginDataEntered(ui.hostField->text(), ui.usernameField->text(), ui.passwordField->text(), true);
        ui.passwordField->setText("");
    });
}
