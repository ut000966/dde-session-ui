#-------------------------------------------------
#
# Project created by QtCreator 2016-06-16T14:05:21
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-offline-upgrader
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget dtkbase

SOURCES += main.cpp\
        frame.cpp \
    deepinwave.cpp \
    dbus/dbusupdatejob.cpp \
    dbus/dbusupdatejobmanager.cpp \
    dialog.cpp

HEADERS  += frame.h \
    deepinwave.h \
    dbus/dbusupdatejob.h \
    dbus/dbusupdatejobmanager.h \
    dialog.h

RESOURCES += \
    images.qrc

target.path = /usr/lib/deepin-daemon/

INSTALLS += target