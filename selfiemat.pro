#-------------------------------------------------
#
# Project created by QtCreator 2015-03-22T16:07:10
#
#-------------------------------------------------

QT       += core
QT       += gui
QT       += widgets
QT       += printsupport

TARGET = selfiemat
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    photo.cpp \
    eye.cpp

HEADERS += \
    mainwindow.h \
    photo.h \
    eye.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libgphoto2
