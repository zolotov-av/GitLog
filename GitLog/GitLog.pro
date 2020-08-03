#-------------------------------------------------
#
# Project created by QtCreator 2019-11-10T16:40:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GitLog
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


SOURCES += \
        main.cpp \
        logwindow.cpp \
    gitlogmodel.cpp \
    QGitLogDelegate.cpp \
    gitcommitfiles.cpp \
    gitdiffprocess.cpp

HEADERS += \
        logwindow.h \
    gitlogmodel.h \
    QGitLogDelegate.h \
    gitcommitfiles.h \
    gitdiffprocess.h

FORMS += \
        logwindow.ui


INCLUDEPATH += $$PWD/../QGit/ $$PWD/../QGit/src/

LIBS += -L$$OUT_PWD/../QGit/ -lQGit

LIBS += -lgit2

target.path = /usr/local/bin
INSTALLS += target
