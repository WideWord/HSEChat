#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <QObject>
#include <QTcpSocket>
#include "../Common/network/Connection.hpp"
#include "../Common/network/messages/Messages.hpp"

class ChatServer;

class ClientHandler : public QObject
{
    Q_OBJECT
    Connection* connection;

    bool loggedIn;
    QString username;
    ChatServer* server;
private:
    void sendContactList();
public:
    explicit ClientHandler(QTcpSocket* socket, ChatServer* server, QObject* parent = 0);

};

#endif // CLIENTHANDLER_HPP
