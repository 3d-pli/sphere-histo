#-------------------------------------------------
#
# Project created by QtCreator 2020-03-12T07:17:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cpp-sphere-histo
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

QMAKE_CXXFLAGS += -std=c++14

MOC_DIR     = build/moc
OBJECTS_DIR = build/obj
UI_DIR      = build/ui
RCC_DIR     = build/


SOURCES += \
        main.cpp \
        spherewidget.cpp \
    icosphere.cpp \
    mainwindow.cpp

HEADERS += \
        spherewidget.h \
    icosphere.h \
    mainwindow.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../external-libraries/cnpy-master/build/release/ -lcnpy
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../external-libraries/cnpy-master/build/debug/ -lcnpy
else:unix: LIBS += -L$$PWD/../external-libraries/cnpy-master/build/ -lcnpy

INCLUDEPATH += $$PWD/../external-libraries/cnpy-master/build
DEPENDPATH += $$PWD/../external-libraries/cnpy-master/build

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../external-libraries/cnpy-master/build/release/libcnpy.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../external-libraries/cnpy-master/build/debug/libcnpy.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../external-libraries/cnpy-master/build/release/cnpy.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../external-libraries/cnpy-master/build/debug/cnpy.lib
else:unix: PRE_TARGETDEPS += $$PWD/../external-libraries/cnpy-master/build/libcnpy.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/release/ -lGLU
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/debug/ -lGLU
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lGLU

INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/release/libGLU.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/debug/libGLU.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/release/GLU.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/debug/GLU.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/libGLU.a

FORMS += \
    mainwindow.ui
