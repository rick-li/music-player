#-------------------------------------------------
#
# Project created by QtCreator 2013-03-16T21:56:35
#
#-------------------------------------------------

QT       += core gui
CONFIG += staticlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = duoPlayer
TEMPLATE = app
include(QMidi/QMidi.pri)

HEADERS  += mainwindow.h

HEADERS  += player.h

SOURCES += main.cpp

SOURCES += mainwindow.cpp
SOURCES += player.cpp


FORMS    += mainwindow.ui
