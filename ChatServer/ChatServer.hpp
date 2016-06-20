#ifndef CHATSERVER_HPP
#define CHATSERVER_HPP

#include <QTcpServer>
#include <QList>
#include "ClientHandler.hpp"

class ChatServer : public QTcpServer {
    Q_OBJECT
public:
    QList<ClientHandler*> clientHandlers;
    ChatServer();
};

#endif // CHATSERVER_HPP
