#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T14:15:25
#
#-------------------------------------------------

QT += core gui
QT += widgets
CONFIG += console
CONFIG += no_batch

#greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = TestIris
TEMPLATE = app

CONFIG += c++14

INCLUDEPATH += "C:/Programming/Light Punch/Neural Network/NeuralNetwork"

include("C:/Programming/Light Punch/Neural Network/NeuralNetwork/NeuralNetwork.pro")

QMAKE_CXXFLAGS += /O2

SOURCES += main.cpp \
    mainwindow.cpp \

HEADERS += mainwindow.h \


FORMS += mainwindow.ui
