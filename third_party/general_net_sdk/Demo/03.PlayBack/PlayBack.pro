#-------------------------------------------------
#
# Project created by QtCreator 2018-10-10T19:11:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlayBack
TEMPLATE = app

SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h \
        dhnetsdk.h \

FORMS    += dialog.ui

win32:LIBS += ../PlayBack/lib/win32/dhnetsdk.lib
win64:LIBS += ../PlayBack/lib/win64/dhnetsdk.lib
unix:LIBS  += -L. -ldhnetsdk -lpthread -Wl,-rpath .
unix:INCPATH  +=../../../Include/Common  ../../Include/Common

