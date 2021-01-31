QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += src/main.cpp \
    src/content_view.cpp \
    src/imageHistory.cpp \
    src/mainwindow.cpp \
    src/tools.cpp \
    src/histogram.cpp \
    src/parameters.cpp \
    src/painter.cpp

HEADERS += hdr/mainwindow.h \
    hdr/constants.h \
    hdr/content_view.h \
    hdr/imageHistory.h \
    hdr/histogram.h \
    hdr/parameters.hpp \
    hdr/parameters.hpp \
    hdr/tools.h \
    hdr/painter.h

FORMS += form/mainwindow.ui \
    form/penwidthdialog.ui \
    form/resizewindow.ui

TRANSLATIONS += \
    CHIMP_en_001.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += resource/StyleResources.qrc \
             resource/icons.qrc

DISTFILES +=
