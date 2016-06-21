#include "Connection.hpp"

Connection::Connection(QTcpSocket* socket, QObject *parent) : QObject(parent)
{
    this->socket = socket;
    socket->setParent(this);
    QObject::connect(socket, &QTcpSocket::readyRead, [this](){
        onSocketReadyRead();
    });
}

void Connection::handleIncomingMessage(const QByteArray& packet) {
    QDataStream stream(packet);
    qint16 messageType;
    stream >> messageType;
    auto handler = incomingMessageHandlers[messageType];
    if (handler != nullptr) {
        handler(stream);
    }
}

void Connection::onSocketReadyRead() {
    buffer.append(socket->readAll());
    while (true) {
        size_t bufferSize = buffer.size();
        if (bufferSize > sizeof(qint32)) {
            size_t packetSize = *((qint32*)buffer.data());
            if (bufferSize - sizeof(qint32) >= packetSize) {
                QByteArray packet = buffer.mid(sizeof(qint32), packetSize);
                buffer = buffer.right(bufferSize - (packetSize + sizeof(qint32)));
                handleIncomingMessage(packet);
            } else {
                break;
            }
        } else {
            break;
        }
    }
}
