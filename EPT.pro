######################################################################
# Automatically generated by qmake (2.01a) Wed Jan 22 13:18:38 2014
######################################################################

TEMPLATE = app
TARGET = ept
DEPENDPATH += .
INCLUDEPATH += . ../Dodge/include
QMAKE_CXXFLAGS = -std=c++0x -DDEBUG -DLINUX -g -DGLEW
QMAKE_LIBDIR = ../Dodge/lib
LIBS += -lDodge -lX11 -lGL -lGLEW

# Input
HEADERS += Application.hpp \
           Common.hpp \
           EptObject.hpp \
           Exporter.hpp \
           Importer.hpp \
           MainWindow.hpp \
           MapSettings.hpp \
           Notifications.hpp \
           ObjectContainer.hpp \
           WgtMapSettings.hpp \
           WgtXmlTreeView.hpp
SOURCES += Application.cpp \
           Common.cpp \
           EptObject.cpp \
           Exporter.cpp \
           Importer.cpp \
           main.cpp \
           MainWindow.cpp \
           MapSettings.cpp \
           Notifications.cpp \
           ObjectContainer.cpp \
           WgtMapSettings.cpp \
           WgtXmlTreeView.cpp
