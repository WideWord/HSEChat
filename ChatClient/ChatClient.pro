#-------------------------------------------------
#
# Project created by QtCreator 2016-06-18T13:49:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatClient
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp \
    ui/LoginDialog.cpp \
    ui/MainWindow.cpp  \
    ../Common/network/Connection.cpp


HEADERS  += \
    ui/LoginDialog.hpp \
    ui/MainWindow.hpp \
    ../Common/network/messages/Messages.hpp \
    ../Common/network/Connection.hpp \
    ../Common/network/Serializer.hpp \
    ../Common/util/TypeIndex.hpp


FORMS    += \
    ui/LoginDialog.ui \
    ui/MainWindow.ui
