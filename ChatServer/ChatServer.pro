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
    ClientHandler.cpp

HEADERS += \
    ChatServer.hpp \
    ClientHandler.hpp

LIBS += -L../Common -lCommon
