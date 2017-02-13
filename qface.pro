#
# Realtime face animation project
#
# @author tangshi
# @date Oct 20th 2016

QT += core widgets gui

TEMPLATE = app

TARGET = qface

include(facedata/facedata.pri)


HEADERS += facewindow.h \
    mainwindow.h

SOURCES += main.cpp \
    facewindow.cpp \
    mainwindow.cpp

RESOURCES += shaders/shaders.qrc

FORMS += mainwindow.ui
