#-------------------------------------------------
#
# Project created by QtCreator 2020-08-24T11:33:50
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ParkSystem
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

CONFIG += c++11

SOURCES += \
        functiontool/advertincharge.cpp \
        functiontool/control.cpp \
        functiontool/flow.cpp \
        functiontool/timer.cpp \
        main.cpp \
        ui/advertchargevideo.cpp \
        ui/masterlogin.cpp \
        ui/masterselect.cpp \
        ui/mastervideo.cpp \
        ui/video.cpp \
        widget.cpp

HEADERS += \
        config.h \
        functiontool/advertincharge.h \
        functiontool/control.h \
        functiontool/flow.h \
        functiontool/timer.h \
        ui/advertchargevideo.h \
        ui/masterlogin.h \
        ui/masterselect.h \
        ui/mastervideo.h \
        ui/video.h \
        widget.h

FORMS += \
        ui/advertchargevideo.ui \
        ui/masterlogin.ui \
        ui/masterselect.ui \
        ui/mastervideo.ui \
        ui/video.ui \
        widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    source/pic.qrc
