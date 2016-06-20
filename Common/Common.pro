#-------------------------------------------------
#
# Project created by QtCreator 2016-06-18T13:52:17
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = Common
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    network/Connection.cpp

HEADERS += \
    network/messages/Messages.hpp \
    network/Connection.hpp \
    network/Serializer.hpp \
    util/TypeIndex.hpp
