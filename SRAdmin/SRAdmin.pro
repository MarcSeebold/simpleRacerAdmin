#-------------------------------------------------
#
# Project created by QtCreator 2015-06-22T16:26:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SRAdmin
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cc\
        MainWindow.cc \
    ClientConnection.cc \
    ClientConnectionManager.cc \
    Common.cc \
    GameSearcher.cc

HEADERS  += MainWindow.hh \
    ClientConnection.hh \
    ClientConnectionManager.hh \
    Common.hh \
    GameSearcher.hh

FORMS    += MainWindow.ui
