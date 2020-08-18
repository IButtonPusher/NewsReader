#-------------------------------------------------
#
# Project cANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sourcested by QtCreator 2016-07-02T11:06:10
#
#-------------------------------------------------

QT       += core gui qml quick webview sql network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Okyesno
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newitemswidget.cpp \
    article.cpp \
    userdata.cpp \
    database.cpp \
    newsswipedisplay.cpp \
    filedownloader.cpp

HEADERS  += mainwindow.h \
    newitemswidget.h \
    article.h \
    userdata.h \
    database.h \
    newsswipedisplay.h \
    filedownloader.h

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    qml.qrc \
    images.qrc

FORMS += \
    mainwindow.ui

DISTFILES += \
    android-sources/AndroidManifest.xml


ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources
