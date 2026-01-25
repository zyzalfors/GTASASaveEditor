QT       += core gui \
            widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    GTASASave.cpp \
    main.cpp \
    mainform.cpp

HEADERS += \
    GTASASave.h \
    mainform.h

FORMS += \
    mainform.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
