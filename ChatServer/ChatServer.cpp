#include "ChatServer.hpp"
#include <QSqlDatabase>
#include <QSqlQuery>

ChatServer::ChatServer() {
    auto database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("data.sqlite");
    database.open();

    QSqlQuery(
                "CREATE TABLE IF NOT EXISTS contacts ( \
                    `user1`	TEXT NOT NULL,\
                    `user2`	TEXT NOT NULL \
                )").exec();
            QSqlQuery("CREATE TABLE IF NOT EXISTS messages ( \
                `sender`	TEXT NOT NULL, \
                `receiver`	TEXT NOT NULL, \
                `date`	INTEGER NOT NULL, \
                `body`	BLOB NOT NULL \
            )").exec();
            QSqlQuery(
                "CREATE TABLE IF NOT EXISTS users ( \
                    `username`	TEXT NOT NULL, \
                    `password`	TEXT NOT NULL, \
                    PRIMARY KEY(username) \
                )").exec();

    connect(this, &QTcpServer::newConnection, [this]() {
        qDebug() << "New incoming connection";
        QTcpSocket* socket = nextPendingConnection();
        ClientHandler* handler = new ClientHandler(socket, this, this);
        clientHandlers.append(handler);
    });
}


