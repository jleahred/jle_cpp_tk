#
#
#
#

QMAKE_CXXFLAGS += -std=c++1z

QT += widgets

TARGET = hpgui
TEMPLATE = app
SOURCES +=     *.cpp \
    ../../../../src/core/*.cpp \
    ../../../../src/core/cont/*.cpp \
    ../../../../src/core/hp/*.cpp

HEADERS += *.h
INCLUDEPATH += ../../../../src/
FORMS += hpgui.ui
RESOURCES += 
