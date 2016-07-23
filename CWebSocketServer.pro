TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

OBJECTS_DIR = ./obj

SOURCES += main.cpp \
    cserver.cpp \
    clog.cpp \
    crequest.cpp \
    cresponse.cpp \
    base64.cpp \
    sha1.cpp \
    cprotocol.cpp

HEADERS += \
    cserver.h \
    clog.h \
    crequest.h \
    cresponse.h \
    base64.h \
    sha1.h \
    cprotocol.h
