QT       += core gui sql network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PayrollMgmtSys
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/cerrorhandler.cpp \
    src/cfoldersetting.cpp \
    src/loginform.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    ../Constant/Connection.h \
    src/CMessage.h \
    src/cerrorhandler.h \
    src/cfoldersetting.h \
    src/loginform.h \
src/mainwindow.h

FORMS += \
    ui/loginform.ui \
    ui/mainwindow.ui

RESOURCES += \
    recources/resources.qrc
