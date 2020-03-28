######################################################################
# Automatically generated by qmake (3.1) Fri Feb 14 17:22:46 2020
######################################################################
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
TARGET = Emulator
INCLUDEPATH += .

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += classes.h
SOURCES += graphics.cpp \
           compiler.cpp \
           control.cpp \
           devices.cpp \
           drivers.cpp \
           main.cpp \
           tests.cpp \
           supportClasses.cpp
