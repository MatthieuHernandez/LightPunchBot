QT       += core
QT       += gui

TARGET = testCuda
CONFIG += console
#CONFIG   -= app_bundle

TEMPLATE = app

#include("C:/Programming/Light Punch/Neural Network/NeuralNetwork/NeuralNetwork.pro")

# Source files
SOURCES += main.cpp

# This makes the .cu files appear in your project
OTHER_FILES +=  vectorAddition.cu

DESTDIR = release
OBJECTS_DIR = release/obj
CUDA_OBJECTS_DIR = release/cuda



CUDA_SOURCES += vectorAddition.cu
CUDA_DIR = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0"            # Path to cuda toolkit install
SYSTEM_NAME = x64                                                               # Depending on your system either 'Win32', 'x64', or 'Win64'
SYSTEM_TYPE = 64                                                                # '32' or '64', depending on your system
CUDA_ARCH = compute_62                                                          # Type of CUDA architecture, for example 'compute_10', 'compute_11', 'sm_10'
NVCC_OPTIONS = --use_fast_math

INCLUDEPATH += "C:/Programming/Light Punch/tests/testCuda" \
               "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0/include" \
               "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin"


LIBS += "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0/lib/x64/*.lib"

QMAKE_LIBDIR += "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0/lib/x64/"


# The following library conflicts with something in Cuda
QMAKE_LFLAGS_RELEASE = /NODEFAULTLIB:msvcrt.lib
QMAKE_LFLAGS_DEBUG   = /NODEFAULTLIB:msvcrtd.lib

QMAKE_CFLAGS_RELEASE += /MT
QMAKE_CXXFLAGS_RELEASE += /MT

# Add the necessary libraries
CUDA_LIBS = cuda cudart

# The following makes sure all path names (which often include spaces) are put between quotation marks
CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')
NVCC_LIBS = $$join(CUDA_LIBS,' -l','-l', '')
#LIBS += $$join(CUDA_LIBS,'.lib ', '', '.lib')


cuda.input = CUDA_SOURCES
cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
cuda.commands = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0/bin/nvcc.exe" $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
cuda.dependency_type = TYPE_C
QMAKE_EXTRA_COMPILERS += cuda
