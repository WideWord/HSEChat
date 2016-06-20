#-------------------------------------------------
#
# Project created by QtCreator 2016-06-18T13:49:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatClient
TEMPLATE = app


SOURCES += main.cpp \
    ui/LoginDialog.cpp \
    ui/MainWindow.cpp

HEADERS  += \
    ui/LoginDialog.hpp \
    ui/MainWindow.hpp

FORMS    += \
    ui/LoginDialog.ui \
    ui/MainWindow.ui

LIBS += -L../Common -lCommon
