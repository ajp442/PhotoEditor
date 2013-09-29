#-------------------------------------------------
#
# Project created by QtCreator 2013-09-19T16:54:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProgrammingAssignment1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    image.cpp

HEADERS  += mainwindow.h \
    mdiChild.h \
    image.h

FORMS    += mainwindow.ui

RESOURCES = ProgrammingAssignment1.qrc

CONFIG += CONSOLE
