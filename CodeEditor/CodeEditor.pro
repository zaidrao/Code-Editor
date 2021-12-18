QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = appic.ico

SOURCES += \
    FindNReplace.cpp \
    Highlighter.cpp \
    about.cpp \
    cStyle.cpp \
    codeeditor.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    FindNReplace.h \
    Highlighter.h \
    MainWindow.h \
    about.h \
    cStyle.h \
    codeeditor.h

FORMS += \
    FindNReplace.ui \
    MainWindow.ui \
    about.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
