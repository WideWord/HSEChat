#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <QObject>
#include <QTcpSocket>
#include "../Common/network/Connection.hpp"
#include "../Common/network/messages/Messages.hpp"

class ClientHandler : public QObject
{
    Q_OBJECT
    Connection* connection;

    bool loggedIn;
    QString username;
private:
    void sendContactList();
public:
    explicit ClientHandler(QTcpSocket* socket, QObject* parent = 0);

};

#endif // CLIENTHANDLER_HPP
