#-------------------------------------------------
#
# Project created by QtCreator 2015-03-27T00:58:55
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fileshare
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    mydb.cpp \
    logviewer.cpp


HEADERS  += mainwindow.h \
    server.h \
    mydb.h \
    logviewer.h

FORMS    += mainwindow.ui
