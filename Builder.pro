#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T10:41:33
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = build_32.ico
VERSION = 1.0.0.1
QMAKE_TARGET_COMPANY = Kontel Co
QMAKE_TARGET_PRODUCT = Relkon Core Builder
QMAKE_TARGET_DESCRIPTION = Relkon Core Builder
QMAKE_TARGET_COPYRIGHT = Kontel

TARGET = Builder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    gitmanager.cpp \
    branchbuilder.cpp \
    rcompiler.cpp

HEADERS  += mainwindow.h \
    settings.h \
    gitmanager.h \
    branchbuilder.h \
    rcompiler.h

FORMS    += mainwindow.ui
