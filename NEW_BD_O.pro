QT += core gui

QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanager.cpp \
    employeedialog.cpp \
   # employeesubscriberwidget.cpp \
    #employeesubscriberdialog.cpp \
    employeewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    officedialog.cpp \
    officewidget.cpp \
    positiondialog.cpp \
    positionwidget.cpp \
    subscriberdialog.cpp \
    subscriberwidget.cpp \
    tariffdialog.cpp \
    tariffwidget.cpp

HEADERS += \
    databasemanager.h \
    employeedialog.h \
   # employeesubscriberwidget.h \
   # employeesubscriberdialog.h \
    employeewidget.h \
    mainwindow.h \
    officedialog.h \
    officewidget.h \
    positiondialog.h \
    positionwidget.h \
    subscriberdialog.h \
    subscriberwidget.h \
    tariffdialog.h \
    tariffwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
