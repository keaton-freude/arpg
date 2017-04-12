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

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(QOgreColorPickerWidget.pri)

FORMS += \
    QOgreColorPickerWidget.ui
