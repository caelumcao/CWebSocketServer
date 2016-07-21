TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cserver.cpp \
    clog.cpp

HEADERS += \
    cserver.h \
    clog.h
