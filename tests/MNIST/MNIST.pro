#-------------------------------------------------
#
# Project created by QtCreator 2017-07-16T15:28:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MNIST
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include("C:/Programming/LightPunchBot/Neural Network/NeuralNetwork/NeuralNetwork.pro")

INCLUDEPATH += $$PWD \
               "C:/Qt/5.10.0/msvc2015_64/bin" \
               "$$PWD/../Neural Network/NeuralNetwork/cuda/bin" \
               "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include" \
               "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um" \
               "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/shared" \
               "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/ucrt" \
#DESTDIR = $$PWD

#QMAKE_CXXFLAGS += -O3

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui
