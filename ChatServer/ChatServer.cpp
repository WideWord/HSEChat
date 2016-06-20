#include "ChatServer.hpp"
#include <QSqlDatabase>


ChatServer::ChatServer() {
    auto database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("data.sqlite");
    database.open();

    connect(this, &QTcpServer::newConnection, [this]() {
        qDebug() << "New incoming connection";
        QTcpSocket* socket = nextPendingConnection();
        ClientHandler* handler = new ClientHandler(socket, this, this);
        clientHandlers.append(handler);
    });
}


