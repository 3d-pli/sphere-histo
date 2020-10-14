#-------------------------------------------------
#
# Project created by QtCreator 2020-03-12T07:17:35
#
#-------------------------------------------------

QT       += core gui
QT += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sphere-histo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -std=c++14 -Wall

MOC_DIR     = build/moc
OBJECTS_DIR = build/obj
UI_DIR      = build/ui
RCC_DIR     = build/

INCLUDEPATH +=  $$PWD/./resources/ #\
#                $$PWD/../external-libraries/cnpy-master/

SOURCES += \
        main.cpp \
        spherewidget.cpp \
        mainwindow.cpp \
    renderdata.cpp \
    spheredepthdata.cpp


HEADERS += \
        spherewidget.h \
        mainwindow.h \
    renderdata.h \
    spheredepthdata.h


FORMS += \
    mainwindow.ui

unix:!macx: LIBS += -L$$PWD/libs/cnpy-master/build/ -lcnpy
