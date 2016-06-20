#pragma once
#include "../Serializer.hpp"
#include "../../util/TypeIndex.hpp"
#include <QString>
#include <QDate>

struct LoginMessage {
    QString username;
    QString password;
    bool signUp;

    void operator%(Serializer& s) {
        username %s;
        password %s;
        signUp %s;
    }
};

enum class LoginResultMessage : qint8 {
    Ok = 0,
    Failure = 1,
};

inline void operator%(LoginResultMessage& msg, Serializer& s) {
    (qint8&)msg %s;
}

struct ContactListMessage {
    QList<QString> users;

    void operator%(Serializer& s) {
        users %s;
    }
};

struct AddContactMessage {
    QString contactUsername;

    void operator%(Serializer& s) {
        contactUsername %s;
    }
};

struct AddContactResultMessage {
    QString contactUsername;
    bool ok;

    void operator%(Serializer& s) {
        contactUsername %s;
        ok %s;
    }
};

struct RemoveContactMessage {
    QString contactUsername;

    void operator%(Serializer& s) {
        contactUsername %s;
    }
};

struct MessagesRequestMessage {
    QString contactUsername;

    void operator%(Serializer& s) {
        contactUsername %s;
    }
};


struct MessagesMessage {

    struct Message {
        QDateTime date;
        QString body;
        bool isMyMessage;
    };

    QString contactUsername;
    QList<MessagesMessage::Message> messages;

    void operator%(Serializer& s) {
        contactUsername %s;
        messages %s;
    }
};

inline QDataStream& operator<<(QDataStream& s, const MessagesMessage::Message& msg) {
    s << msg.date;
    s << msg.body;
    s << msg.isMyMessage;
    return s;
}

inline QDataStream& operator>>(QDataStream& s, MessagesMessage::Message& msg) {
    s >> msg.date;
    s >> msg.body;
    s >> msg.isMyMessage;
    return s;
}

struct NewMessageMessage {
    QDateTime date;
    QString body;
    QString sender;
    QString receiver;

    void operator%(Serializer& s) {
        date %s;
        body %s;
        sender %s;
        receiver %s;
    }
};


template<typename M> using MessageTypeIndex = TypeIndex<M,
LoginMessage,
LoginResultMessage,
ContactListMessage,
AddContactMessage,
AddContactResultMessage,
RemoveContactMessage,
MessagesRequestMessage,
MessagesMessage,
NewMessageMessage
>;
