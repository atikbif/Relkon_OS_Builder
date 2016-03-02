#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T10:41:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Builder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    gitmanager.cpp \
    branchbuilder.cpp

HEADERS  += mainwindow.h \
    settings.h \
    gitmanager.h \
    branchbuilder.h

FORMS    += mainwindow.ui
