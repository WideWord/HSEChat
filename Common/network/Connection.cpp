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
        auto bufferSize = buffer.size();
        if (bufferSize > sizeof(qint32)) {
            qint32 packetSize = *((qint32*)buffer.data());
            if (bufferSize - sizeof(qint32) >= packetSize) {
                QByteArray packet = buffer.mid(sizeof(qint32), packetSize);
                buffer = buffer.right(bufferSize - (packetSize + sizeof(qint32)));

                qDebug() << packet.toHex();
                handleIncomingMessage(packet);
            } else {
                break;
            }
        } else {
            break;
        }
    }
}
