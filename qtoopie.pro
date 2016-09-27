#-------------------------------------------------
#
# Project created by QtCreator 2016-09-24T22:19:27
#
#-------------------------------------------------

QT       += core gui network
LIBS 	 +=-lboost_system

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qtoopie
TEMPLATE = app


SOURCES += main.cpp\
        main_window.cpp \
    i2pcontrol_client.cpp \
    sidebar.cpp \
    mainpage.cpp \
    qcustomplot.cpp

HEADERS  += main_window.h \
    i2pcontrol_client.h \
    sidebar.h \
    mainpage.h \
    qcustomplot.h

FORMS    += main_window.ui \
    sidebar.ui \
    mainpage.ui
