QT       += core gui sql network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PayrollMgmtSys
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/cerrorhandler.cpp \
    src/cfoldersetting.cpp \
    src/client.cpp \
    src/loginform.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/databaseutils.cpp \
    src/employeeinfo.cpp \
    src/employeedbutils.cpp \
    src/techused.cpp \
    src/about.cpp

HEADERS += \
    src/CMessage.h \
    src/cerrorhandler.h \
    src/cfoldersetting.h \
    src/client.h \
    src/loginform.h \
        src/mainwindow.h \
    src/employeeinfo.h \
    src/techused.h \
    src/about.h

FORMS += \
    ui/loginform.ui \
    ui/mainwindow.ui \
    ui/employeeinfo.ui \
    ui/techused.ui \
    ui/about.ui

RESOURCES += \
    recources/resources.qrc
