#-------------------------------------------------
#
# Project created by QtCreator 2013-03-21T13:26:19
#
#-------------------------------------------------

QT       += core gui opengl
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameOfLifeOpenGL
TEMPLATE = app


SOURCES += main.cpp \
    openglwindow.cpp

HEADERS  += \
    openglwindow.h
