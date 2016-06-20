#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QMap>
#include "messages/Messages.hpp"
#include "../util/TypeIndex.hpp"
#include <functional>

class Connection : public QObject
{
    Q_OBJECT

    QTcpSocket *socket;
    QMap<qint16, std::function<void(QDataStream&)>> incomingMessageHandlers;

    QByteArray buffer;

    void handleIncomingMessage(const QByteArray& packet);
    void onSocketReadyRead();
public:
    explicit Connection(QTcpSocket *socket, QObject *parent = 0);

    template<typename M> void sendMessage(M msg) {
        QByteArray packet;
        QDataStream dataStream(&packet, QIODevice::WriteOnly);
        qint16 messageType = MessageTypeIndex<M>::index;
        dataStream << messageType;
        Serializer s(dataStream, true);
        msg %s;
        qint32 packetSize = packet.size();
        socket->write((char*)&packetSize, sizeof(packetSize));
        socket->write(packet);
    }

    template<typename M> void setIncomingMessageHandler(std::function<void(const M&)> handler) {
        qint16 messageType = MessageTypeIndex<M>::index;
        if (handler == nullptr) {
            incomingMessageHandlers[messageType] = nullptr;
        } else {
            auto preparedHandler = [handler](QDataStream& stream) {
                Serializer s(stream, false);
                M message;
                message %s;
                handler(message);
            };
            incomingMessageHandlers[messageType] = preparedHandler;
        }
    }
};

#endif // CONNECTION_HPP
