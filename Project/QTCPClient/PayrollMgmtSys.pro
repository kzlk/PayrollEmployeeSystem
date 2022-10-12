QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PayrollMgmtSys
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    cerrorhandler.cpp \
    client.cpp \
    loginform.cpp \
        main.cpp \
        mainwindow.cpp \
    databaseutils.cpp \
    employeeinfo.cpp \
    employeedbutils.cpp \
    techused.cpp \
    about.cpp

HEADERS += \
    CMessage.h \
    cerrorhandler.h \
    client.h \
    loginform.h \
        mainwindow.h \
    employeeinfo.h \
    techused.h \
    about.h

FORMS += \
    loginform.ui \
        mainwindow.ui \
    employeeinfo.ui \
    techused.ui \
    about.ui

RESOURCES += \
    resources.qrc
