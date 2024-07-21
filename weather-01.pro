QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT       += network

greaterThan(QT_MAJOR_VERSION, 5): QT +=  core5compat
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    citydialog.cpp \
    main.cpp \
    mainwindow.cpp \
    predictwnd.cpp \
    qweatherstyleditemdelegate.cpp \
    weather.cpp

HEADERS += \
    citydialog.h \
    mainwindow.h \
    predictwnd.h \
    qweatherstyleditemdelegate.h \
    weather.h

FORMS += \
    citydialog.ui \
    mainwindow.ui \
    predictwnd.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
