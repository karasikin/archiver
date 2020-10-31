QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    archiver.cpp \
    bwt.cpp \
    huffman.cpp \
    huffmantree.cpp \
    main.cpp \
    mtf.cpp \
    widget.cpp

HEADERS += \
    archiver.h \
    bwt.h \
    byteconverter.h \
    exception.h \
    huffman.h \
    huffmantree.h \
    mtf.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
