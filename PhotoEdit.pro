QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoEdit
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mdichild.cpp \
    image.cpp \
    dialog.cpp \

HEADERS  += mainwindow.h \
    mdichild.h \
    image.h \
    dialog.h \

RESOURCES += \
    PhotoEdit.qrc
