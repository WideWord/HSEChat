#ifndef LOGINDIALOG_HPP
#define LOGINDIALOG_HPP

#include "ui_LoginDialog.h"

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = 0);
private:
    Ui::LoginDialog ui;
signals:
    void loginDataEntered(const QString& host, const QString& username, const QString& password, bool signup);
};

#endif // LOGINDIALOG_HPP
