#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_MainWindow.h"
#include "LoginDialog.hpp"
#include "../Common/network/Connection.hpp"
#include "../Common/network/messages/Messages.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTcpSocket* socket;
    Connection* connection;
    QLabel* statusLabel;
    QMap<QString, QString> messagesCache;
    void resetState();

    LoginMessage loginMessage;
public:
    LoginDialog loginDialog;
    explicit MainWindow(QWidget *parent = 0);
protected:
    virtual void showEvent(QShowEvent *ev) override;
private:
    Ui::MainWindow ui;
};

#endif // MAINWINDOW_HPP
