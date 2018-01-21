#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T16:06:27
#
#-------------------------------------------------

QT += core gui

QT += widgets # greaterThan(QT_MAJOR_VERSION, 4):

QT += sql


TEMPLATE = app
CONFIG += c++14


INCLUDEPATH += "$$PWD" \
    "$$PWD/../" \
    "$$PWD/../database/sqlite-amalgamation-3150000" \
    "$$PWD/../picture" \
    "C:/Qt/5.10.0/msvc2015_64/bin" \
    "$$PWD/../Neural Network/NeuralNetwork/cuda/bin" \
    "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include" \
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um" \
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/shared"

#DEPENDPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um" \

#LIBS += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um/*.h" \

include("C:/Programming/LightPunchBot/Neural Network/NeuralNetwork/NeuralNetwork.pro")

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
    gui/neural_network_card.cpp \
    ai/ongoing_action.cpp \
    lib/qcustomplot.cpp \
    environnement/database.cpp \
    environnement/database_manager.cpp \
    main.cpp \
    main.cpp

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
    gui/neural_network_card.h \
    ai/ongoing_action.h \
    lib/qcustomplot.h \
    environnement/database.h \
    environnement/database_manager.h \
    parameters.h

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
