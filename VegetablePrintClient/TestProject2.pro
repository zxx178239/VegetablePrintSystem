#-------------------------------------------------
#
# Project created by QtCreator 2018-06-25T15:44:28
#
#-------------------------------------------------

QT       += core gui

QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestProject2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS    += -lWs2_32

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    vegetableprintwidget.cpp \
    searchwidget.cpp \
    datamanager.cpp \
    socketclient.cpp

HEADERS += \
        mainwindow.h \
    vegetableprintwidget.h \
    searchwidget.h \
    common.h \
    datamanager.h \
    socketclient.h \
    messagedefine.h

FORMS += \
        mainwindow.ui \
    vegetableprintwidget.ui \
    searchwidget.ui
