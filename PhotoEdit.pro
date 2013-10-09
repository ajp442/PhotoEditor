QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoEdit
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mdichild.cpp \
    image.cpp \
    dialog.cpp \
    picturelabel.cpp

HEADERS  += mainwindow.h \
    mdichild.h \
    image.h \
    dialog.h \
    picturelabel.h

RESOURCES += \
    PhotoEdit.qrc
