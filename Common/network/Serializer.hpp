#pragma once

#include <QDataStream>

class Serializer {
private:
    QDataStream& stream;
    bool writing;
public:
    Serializer(QDataStream& stream, bool isWriting) : stream(stream), writing(isWriting) {}

    template<typename T> void serialize(T& value) {
        if (writing) {
            stream << value;
        } else {
            stream >> value;
        }
    }

    bool isWriting() {
        return writing;
    }
};

template<typename T> inline void operator%(T& value, Serializer& serializer) {
    serializer.serialize(value);
}

template<typename T> inline void operator%(QList<T>& value, Serializer& s) {
    if (s.isWriting()) {
        quint16 size = value.size();
        size %s;

        for (T& elem : value) {
            elem %s;
        }
    } else {
        quint16 size;
        size %s;

        value.clear();
        for (int i = 0; i < size; ++i) {
            T elem;
            elem %s;
            value.append(elem);
        }
    }
}

