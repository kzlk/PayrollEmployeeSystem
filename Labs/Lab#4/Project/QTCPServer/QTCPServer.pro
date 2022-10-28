QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTCPServer
TEMPLATE = app

CONFIG+=sdk_no_version_check

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

SOURCES += \
    src/cautopilot.cpp \
    src/cgeneratepdf.cpp \
    src/cpayment.cpp \
    src/dbUtils.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    ../Constant/Connection.h \
    src/CMessage.h \
    src/cautopilot.h \
    src/cgeneratepdf.h \
    src/cpayment.h \
    src/mainwindow.h

FORMS += \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

else: unix:!android: target.path = /opt/$${TARGET}/bin
