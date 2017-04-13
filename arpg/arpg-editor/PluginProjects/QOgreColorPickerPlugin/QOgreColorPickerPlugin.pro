CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(QOgreColorPickerWidgetPlugin)
TEMPLATE    = lib

HEADERS     = QOgreColorPickerWidgetPlugin.h
SOURCES     = QOgreColorPickerWidgetPlugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

INSTALLS    += target

include(QOgreColorPickerWidget.pri)

INCLUDEPATH += /usr/local/include/OGRE


FORMS += \
    QOgreColorPickerWidget.ui
