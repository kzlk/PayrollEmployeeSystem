QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTCPServer
TEMPLATE = app

CONFIG+=sdk_no_version_check

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

SOURCES += \
    cautopilot.cpp \
    cgeneratepdf.cpp \
    cpayment.cpp \
    dbUtils.cpp \
        main.cpp \
    mainwindow.cpp

HEADERS += \
    CMessage.h \
    cautopilot.h \
    cgeneratepdf.h \
    cpayment.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
