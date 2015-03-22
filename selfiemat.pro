#-------------------------------------------------
#
# Project created by QtCreator 2015-03-22T16:07:10
#
#-------------------------------------------------

QT       += core
QT       += gui
QT       += widgets

TARGET = selfiemat
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libgphoto2
