QT       += core gui
#QT += 3drender
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += eigen/eigen-3.4.0\
    G:\EtienneArea\Personnel\cgf\eigen-3.3.8

SOURCES += \
    facet.cpp \
    main.cpp \
    mainwndgendesign.cpp \
    mesh.cpp \
    meshevolver.cpp \
    meshgenerator.cpp \
    volume.cpp

HEADERS += \
    facet.h \
    mainwndgendesign.h \
    mesh.h \
    meshevolver.h \
    meshgenerator.h \
    volume.h

FORMS += \
    mainwndgendesign.ui

TRANSLATIONS += \
    GeneticDesign_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
