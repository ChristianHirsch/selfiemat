#-------------------------------------------------
#
# Project created by QtCreator 2015-03-22T16:07:10
#
#-------------------------------------------------

QT       += core
QT       += gui
QT       += widgets
QT       += printsupport
QT       += xml

TARGET = selfiemat
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

inst.path = /opt/selfiemat
inst.files += selfiemat
inst.files += icons
INSTALLS += inst

SOURCES += main.cpp \
    mainwindow.cpp \
    photo.cpp \
    eye.cpp \
    scene.cpp \
    common.cpp \
    screen.cpp \
    focus.c

HEADERS += \
    mainwindow.h \
    photo.h \
    eye.h \
    scene.h \
    common.h \
    screen.h \
    focus.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libgphoto2
