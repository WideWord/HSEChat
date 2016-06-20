#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_MainWindow.h"
#include "LoginDialog.hpp"
#include "../Common/network/Connection.hpp"
#include "../Common/network/messages/Messages.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    LoginDialog loginDialog;
    QTcpSocket* socket;
    Connection* connection;
    QLabel* statusLabel;
    QMap<QString, QString> messagesCache;

    LoginMessage loginMessage;
public:
    explicit MainWindow(QWidget *parent = 0);
protected:
    virtual void showEvent(QShowEvent *ev) override;
private:
    Ui::MainWindow ui;
};

#endif // MAINWINDOW_HPP
