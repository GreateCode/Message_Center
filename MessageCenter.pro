#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T15:20:50
#
#-------------------------------------------------

QT       += core gui xml network dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MessageCenter
TEMPLATE = app


SOURCES += main.cpp\
    btnmodel.cpp \
    buttonlabel.cpp \
    msgwidget.cpp \
    pagewidget.cpp \
    titlewidget.cpp \
    getmsg.cpp \
    httpmsg.cpp \
    adaemon.cpp \
    shadowwidget.cpp \
    readconf.cpp \
    backupinfo.cpp \
    actionexpand.cpp \
    mainwidget.cpp \
    tipwidget.cpp \
    lookonemsg.cpp

HEADERS  += \
    btnmodel.h \
    buttonlabel.h \
    msgwidget.h \
    pagewidget.h \
    titlewidget.h \
    getmsg.h \
    httpmsg.h \
    adaemon.h \
    shadowwidget.h \
    readconf.h \
    backupinfo.h \
    actionexpand.h \
    mainwidget.h \
    tipwidget.h \
    lookonemsg.h

FORMS += \
    mainwidget.ui \
    tipwidget.ui \
    lookonemsg.ui

RESOURCES += \
    img.qrc

OTHER_FILES +=
