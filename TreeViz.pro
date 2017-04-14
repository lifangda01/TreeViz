#-------------------------------------------------
#
# Project created by QtCreator 2015-10-15T16:40:26
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TreeViz
TEMPLATE = app
VPATH = ./src \
        ./gui
INCLUDEPATH += /home/fangda/Libraries/assimp-3.1.1/include \

SOURCES += main.cpp\
    mainwindow.cpp \
    myglwidget.cpp\
    functions.cpp\
    severity.cpp \
    readinput.cpp \
    mypicwidget.cpp \
    mldwindow.cpp \
    modelloader.cpp \
    ltrdialog.cpp


HEADERS  += mainwindow.h \
    myglwidget.h\
    usda.h\
    branch.h\
    init.h\
    functions.h\
    severity.h \
    readinput.h \
    mypicwidget.h \
    mldwindow.h \
    modelloader.h \
    camera.h \
    ltrdialog.h


FORMS    += mainwindow.ui \
    mldwindow.ui \
    ltrdialog.ui

DISTFILES += \
    objectfrag.fsh \
    objectvert.vsh \
    cubemap.frag \
    cubemap.vert \
    plane.frag \
    plane.vert \
    stencil.frag \
    stencil.vert \
    shadow.vert \
    shadow.frag

unix: !macx {
    INCLUDEPATH +=  /usr/include
    LIBS += /usr/lib/libassimp.so.3
}


LIBS += -lglut -lGLU

QMAKE_CXXFLAGS += -Wunused-variable -Wunused-parameter -Wsign-compare
