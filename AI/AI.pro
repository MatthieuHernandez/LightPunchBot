#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T16:06:27
#
#-------------------------------------------------

QT += core gui

QT += widgets # greaterThan(QT_MAJOR_VERSION, 4):

QT += sql

QT += printsupport

TEMPLATE = app

CONFIG += c++14

#QT += console

#DEPENDPATH += "$$PWD/../Neural Network/" \

INCLUDEPATH += "$$PWD/../" \
    "$$PWD/../database/sqlite-amalgamation-3150000" \
    "$$PWD/../picture" \
    "C:/Programming/Qt/5.6/mingw49_32/bin" \
    "$$PWD/../Neural Network/NeuralNetwork" \
    "$$PWD/../Neural Network/NeuralNetwork/cuda/bin"

include("C:/Programming/Light Punch/Neural Network/NeuralNetwork/NeuralNetwork.pro")

TARGET = AI

SOURCES += main.cpp\
    environnement/controler.cpp \
    environnement/data_recovery.cpp \
    environnement/player.cpp \
    gui/mainwindow.cpp \
    environnement/read_memory.cpp \
    "$$PWD/../database/sqlite-amalgamation-3150000/sqlite3.c" \
    gui/thread.cpp \
    ai/algorithm.cpp \
    ai/neural_network.cpp \
    ai/neural_network_manager.cpp \
#    "$$PWD/../Neural Network/NeuralNetwork/perceptron.cpp" \
#    "$$PWD/../Neural Network/NeuralNetwork/neuralNetwork.cpp" \
    gui/neural_network_card.cpp \
    ai/ongoing_action.cpp \
    lib/qcustomplot.cpp \
    environnement/database.cpp \
    environnement/database_manager.cpp

HEADERS += parameters.h \
    environnement/controler.h \
    environnement/data_recovery.h \
    environnement/player.h \
    gui/mainwindow.h \
    environnement/read_memory.h \
    "$$PWD/../database/sqlite-amalgamation-3150000/sqlite3.h" \
    gui/thread.h \
    ai/algorithm.h \
    ai/neural_network_manager.h \
    ai/neural_network.h \
#    "$$PWD/../Neural Network/NeuralNetwork/perceptron.h" \
#    "$$PWD/../Neural Network/NeuralNetwork/neuralNetwork.h" \
    gui/neural_network_card.h \
    ai/ongoing_action.h \
    lib/qcustomplot.h \
    environnement/database.h \
    environnement/database_manager.h

FORMS += mainwindow.ui \
    gui/neural_network_card.ui

LIBS += -lpsapi \

RESOURCES += \
    ressource.qrc

if(msvc) {
    message("MSVC")
    QMAKE_CXXFLAGS += /O2
}
if(mingw) {
    message("MinGW")
    QMAKE_CXXFLAGS += -O3
}

DISTFILES += \
    uml_diagram.qmodel


