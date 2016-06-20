#include "ClientHandler.hpp"
#include <QSqlQuery>
#include "ChatServer.hpp"

ClientHandler::ClientHandler(QTcpSocket* socket, ChatServer* _server, QObject* parent) : QObject(parent) {
    connection = new Connection(socket, this);
    server = _server;

    connection->setIncomingMessageHandler<LoginMessage>([this](const LoginMessage& msg) {
        qDebug() << "Login message";

        if (msg.signUp) {
            QSqlQuery query("SELECT username FROM users WHERE username=?");
            query.addBindValue(msg.username);
            query.exec();
            if (query.next()) {
                loggedIn = false;
                qDebug() << "Signup Failure";
                connection->sendMessage(LoginResultMessage::Failure);
            } else {
                QSqlQuery insertQuery("INSERT INTO users(username, password) VALUES(?, ?)");
                insertQuery.addBindValue(msg.username);
                insertQuery.addBindValue(msg.password);
                insertQuery.exec();
                loggedIn = true;
                username = msg.username;
                qDebug() << "Signup Ok";
                connection->sendMessage(LoginResultMessage::Ok);
            }
        } else {
            QSqlQuery query("SELECT username FROM users WHERE username=? AND password=?");
            query.addBindValue(msg.username);
            query.addBindValue(msg.password);
            query.exec();
            if (query.next()) {
                loggedIn = true;
                username = msg.username;
                qDebug() << "Login Ok";
                connection->sendMessage(LoginResultMessage::Ok);
                sendContactList();
            } else {
                loggedIn = false;
                qDebug() << "Login Failure";
                connection->sendMessage(LoginResultMessage::Failure);
            }
        }
    });

    connection->setIncomingMessageHandler<AddContactMessage>([this](const AddContactMessage& msg) {
        if (!loggedIn) return;

        if (msg.contactUsername == username) {
            AddContactResultMessage res;
            res.contactUsername = msg.contactUsername;
            res.ok = false;
            connection->sendMessage(res);
            return;
        }

        QSqlQuery query("SELECT username FROM users WHERE username=?");
        query.addBindValue(msg.contactUsername);
        query.exec();
        if (!query.next()) {
            AddContactResultMessage res;
            res.contactUsername = msg.contactUsername;
            res.ok = false;
            connection->sendMessage(res);
            return;
        }

        query = QSqlQuery("SELECT * FROM contacts WHERE (user1=? AND user2=?) OR (user1=? AND user2=?)");
        query.addBindValue(username);
        query.addBindValue(msg.contactUsername);
        query.addBindValue(msg.contactUsername);
        query.addBindValue(username);
        query.exec();
        if (query.next()) {
            AddContactResultMessage res;
            res.contactUsername = msg.contactUsername;
            res.ok = false;
            connection->sendMessage(res);
            return;
        }

        query = QSqlQuery("INSERT INTO contacts(user1, user2) VALUES (?, ?)");
        query.addBindValue(username);
        query.addBindValue(msg.contactUsername);
        query.exec();

        AddContactResultMessage res;
        res.contactUsername = msg.contactUsername;
        res.ok = true;
        connection->sendMessage(res);

        for (auto& client : server->clientHandlers) {
            if (!client->loggedIn) continue;
            if (client->username == username || client->username == msg.contactUsername) {
                client->sendContactList();
            }
        }
    });

    connection->setIncomingMessageHandler<RemoveContactMessage>([this](const RemoveContactMessage& msg) {
        if (!loggedIn) return;


        QSqlQuery query("DELETE FROM contacts WHERE (user1=? AND user2=?) OR (user1=? AND user2=?)");
        query.addBindValue(username);
        query.addBindValue(msg.contactUsername);
        query.addBindValue(msg.contactUsername);
        query.addBindValue(username);
        query.exec();

        for (auto& client : server->clientHandlers) {
            if (!client->loggedIn) continue;
            if (client->username == username || client->username == msg.contactUsername) {
                client->sendContactList();
            }
        }
    });
}

void ClientHandler::sendContactList() {
    if (!loggedIn) return;


    ContactListMessage msg;

    QSqlQuery query("SELECT user1, user2 FROM contacts WHERE user1=? OR user2=?");
    query.addBindValue(username);
    query.addBindValue(username);
    query.exec();
    while (query.next()) {
        QString user1 = query.value(0).toString();
        QString user2 = query.value(1).toString();
        if (user1 == username) {
            msg.users.append(user2);
        } else {
            msg.users.append(user1);
        }
    }

    connection->sendMessage(msg);
}
