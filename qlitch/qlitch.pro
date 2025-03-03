QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qlitch
TEMPLATE = app

TRANSLATIONS = qlitch_de.ts

CONFIG += exceptions

win32 {
CONFIG += embed_manifest_exe
}

QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64

SOURCES += main.cpp\
    mainwindow.cpp \
    imagewidget.cpp \
    random/rnd.cpp \
    random/mersenne_twister.cpp \
    qlitchapplication.cpp

HEADERS  += mainwindow.h \
    imagewidget.h \
    random/rnd.h \
    random/mersenne_twister.h \
    random/abstract_random_number_generator.h \
    qlitchapplication.h

FORMS += mainwindow.ui

RESOURCES += \
    qlitch.qrc

OTHER_FILES += \
    qlitch_de.ts
