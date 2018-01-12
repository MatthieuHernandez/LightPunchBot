#-------------------------------------------------
#
# Project created by QtCreator 2016-08-12T05:59:12
#
#-------------------------------------------------

QT += core
QT += gui
QT += widgets

TARGET = NeuralNetwork

TEMPLATE = app

CONFIG += console
CONFIG += c++14



SOURCES += main.cpp \
    $$PWD/neuralNetworkForCPU.cpp \
    $$PWD/neuralNetwork.cpp \
    $$PWD/perceptron.cpp \
    $$PWD/neuralNetworkGettersAndSetters.cpp

HEADERS += $$PWD/perceptron.h \
    $$PWD/neuralNetworkForCPU.h \
    $$PWD/neuralNetwork.h \
    $$PWD/optimization.cuh


DESTDIR = $$PWD/release
OBJECTS_DIR = $$PWD/release/obj
CUDA_OBJECTS_DIR = $$PWD/release/cuda


CUDA_SOURCES += $$PWD/optimization.cu
CUDA_DIR = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.1"            # Path to cuda toolkit install
SYSTEM_NAME = x64                                                               # Depending on your system either 'Win32', 'x64', or 'Win64'
SYSTEM_TYPE = 64                                                                # '32' or '64', depending on your system
CUDA_ARCH = compute_50                                                          # Type of CUDA architecture, for example 'compute_10', 'compute_11', 'sm_10'
CUDA_CODE = compute_50,sm_50
NVCC_OPTIONS = --use_fast_math -O3

INCLUDEPATH += "C:/Programming/LightPunchBot/Neural Network/NeuralNetwork" \
               "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.1/include" \
               "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/ucrt" \
               #"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/" \
               ########"C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin" \

DEPENDPATH += "optimization.cu" \
              "C:/Programming/LightPunchBot/Neural Network/NeuralNetwork" \
              "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.1/include" \
              #"C:/Program Files (x86)/Windows Kits/10/Lib\10.0.16299.0/ucrt/arm64" \
              #"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/" \
              #########"C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin" \ # Why I was include this path ?

LIBS += "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.1/lib/x64/*.lib" \
        #"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/*.lib" \

QMAKE_LIBDIR += "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.1/lib/x64/" \
                "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/" \
                "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/ucrt/x64" \


QMAKE_CFLAGS_RELEASE += /MT
QMAKE_CXXFLAGS_RELEASE += /MT

QMAKE_CFLAGS_DEBUG += /MTd
QMAKE_CXXFLAGS_DEBUG += /MTd

# Add the necessary libraries
CUDA_LIBS = cuda cudart

# The following makes sure all path names (which often include spaces) are put between quotation marks
CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')
NVCC_LIBS = $$join(CUDA_LIBS,' -l','-l', '')
#LIBS += $$join(CUDA_LIBS,'.lib ', '', '.lib')


CONFIG(release, debug|release) {

    QMAKE_CXXFLAGS += /O2
    CUDA_DEBUG = ""
    QMAKE_EXTRA_COMPILERS += cuda
    QMAKE_LFLAGS_RELEASE += /NODEFAULTLIB:msvcrt.lib \

}
CONFIG(debug, debug|release) {

    QMAKE_CXXFLAGS += /O0
    DEFINES += _DEBUG
    CUDA_DEBUG = "-D_DEBUG -G"
    QMAKE_EXTRA_COMPILERS += cuda
    QMAKE_LFLAGS_DEBUG   += /NODEFAULTLIB:msvcrtd.lib \
                            /NODEFAULTLIB:libcmt.lib

    cuda_d.input = CUDA_SOURCES
#    cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
}

cuda.input = CUDA_SOURCES
cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
cuda.commands = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.1/bin/nvcc.exe" $$CUDA_DEBUG $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE --gpu-architecture=$$CUDA_ARCH --gpu-code=$$CUDA_CODE -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
cuda.dependency_type = TYPE_C
