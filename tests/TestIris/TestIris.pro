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

INCLUDEPATH += "C:/Programming/LightPunchBot/Neural Network/NeuralNetwork" \
                "C:/Qt/5.10.0/msvc2015_64/bin" \
                "$$PWD/../Neural Network/NeuralNetwork/cuda/bin" \
                "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include" \
                "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um" \
                "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/shared" \
                "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/ucrt"

include("C:/Programming/LightPunchBot/Neural Network/NeuralNetwork/NeuralNetwork.pro")

QMAKE_CXXFLAGS += /O2

SOURCES += main.cpp \
    mainwindow.cpp \

HEADERS += mainwindow.h \

LIBS += -lpsapi \

FORMS += mainwindow.ui
