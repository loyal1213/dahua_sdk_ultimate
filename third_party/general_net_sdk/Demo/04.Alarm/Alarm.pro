#-------------------------------------------------
#
# Project created by QtCreator 2018-06-28T11:26:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Alarm
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h \
    dhnetsdk.h

FORMS    += dialog.ui

win32:LIBS += D:\SVNcode\QtProj\RealPlay\lib\dhnetsdk.lib
#unix:LIBS  += -L. -ldhdvr -ldhnetsdk -lpthread
unix:LIBS  += -L. -ldhnetsdk -lpthread
unix:INCPATH  +=../../../Include/Common ../../Include/Common
