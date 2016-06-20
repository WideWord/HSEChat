#include <QCoreApplication>
#include "ChatServer.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    ChatServer server;
    if (server.listen(QHostAddress::Any, 8003)) {
        qDebug() << "Listening...";
    } else {
        qDebug() << "Error";
    }

    return a.exec();
}
