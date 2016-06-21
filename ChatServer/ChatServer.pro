QT += core
QT += network
QT += sql
QT -= gui

CONFIG += c++11

TARGET = ChatServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ChatServer.cpp \
    ClientHandler.cpp \
    ../Common/network/Connection.cpp


HEADERS += \
    ChatServer.hpp \
    ClientHandler.hpp \
    ../Common/network/messages/Messages.hpp \
    ../Common/network/Connection.hpp \
    ../Common/network/Serializer.hpp \
    ../Common/util/TypeIndex.hpp
