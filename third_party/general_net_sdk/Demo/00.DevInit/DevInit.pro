#-------------------------------------------------
#
# Project created by QtCreator 2018-06-08T10:46:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DevInit
TEMPLATE = app


SOURCES += main.cpp\
        DevInitDlg.cpp\
        InitializeDevDlg.cpp\
        GetIPDlg.cpp \
    NetCardInfo.cpp

HEADERS  += DevInitDlg.h\
        InitializeDevDlg.h\
        GetIPDlg.h\
        dhnetsdk.h \
    NetCardInfo.h

FORMS    += DevInitDlg.ui\
        InitializeDevDlg.ui\
        GetIPDlg.ui

TRANSLATIONS += CHN.ts

win32:LIBS += ../DevInit/lib/win32/dhnetsdk.lib
unix:LIBS  += -L. -ldhnetsdk -lpthread -Wl,-rpath .
unix:INCPATH  +=../../../Include/Common  ../../Include/Common
