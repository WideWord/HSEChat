#pragma once
#include "../Serializer.hpp"
#include "../../util/TypeIndex.hpp"
#include <QString>

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

struct RemoveContactMessage {
    QString contactUsername;

    void operator%(Serializer& s) {
        contactUsername %s;
    }
};

template<typename M> using MessageTypeIndex = TypeIndex<M,
LoginMessage,
LoginResultMessage,
ContactListMessage,
AddContactMessage,
RemoveContactMessage
>;
