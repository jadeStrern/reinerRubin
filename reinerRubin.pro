#-------------------------------------------------
#
# Project created by QtCreator 2013-05-11T14:59:14
#
#-------------------------------------------------

QT       += core gui

INCLUDEPATH += "/usr/include/QtGStreamer/"

# Tell qmake to use pkg-config to find QtGStreamer.
CONFIG += link_pkgconfig

# Now tell qmake to link to QtGStreamer and also use its include path and Cflags.
PKGCONFIG += QtGStreamer-0.10 QtGStreamerUi-0.10# glibmm-2.4

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = reinerRubin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    player.cpp \
    sender.cpp \
    levelinfo.cpp \
    stationwidgetmanager.cpp \
    playermanager.cpp \
    sendermanager.cpp \
    senderwidgetmanager.cpp

HEADERS  += mainwindow.h \
    player.h \
    sender.h \
    levelinfo.h \
    stationwidgetmanager.h \
    playermanager.h \
    sendermanager.h \
    senderwidgetmanager.h

FORMS    += mainwindow.ui

#CONFIG += no_keywords