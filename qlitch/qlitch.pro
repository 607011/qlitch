#-------------------------------------------------
#
# Project created by QtCreator 2013-10-28T10:55:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qlitch
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagewidget.cpp \
    random/rnd.cpp \
    random/mersenne_twister.cpp

HEADERS  += mainwindow.h \
    imagewidget.h \
    main.h \
    random/rnd.h \
    random/mersenne_twister.h \
    random/abstract_random_number_generator.h

FORMS    += mainwindow.ui