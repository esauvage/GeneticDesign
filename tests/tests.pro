QT += testlib
#QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
INCLUDEPATH += ../eigen/eigen-3.4.0\
    G:\EtienneArea\Personnel\cgf\eigen-3.3.8

SOURCES +=  tst_testvolume.cpp \
    ../facet.cpp \
    ../volume.cpp \
    testfacet.cpp

HEADERS += \
    ../facet.h \
    ../volume.h \
    testfacet.h
