#pragma once

#include <QDataStream>

class Serializer {
private:
    QDataStream& stream;
    bool isWriting;
public:
    Serializer(QDataStream& stream, bool isWriting) : stream(stream), isWriting(isWriting) {}

    template<typename T> void serialize(T& value) {
        if (isWriting) {
            stream << value;
        } else {
            stream >> value;
        }
    }
};

template<typename T> void operator%(T& value, Serializer& serializer) {
    serializer.serialize(value);
}
