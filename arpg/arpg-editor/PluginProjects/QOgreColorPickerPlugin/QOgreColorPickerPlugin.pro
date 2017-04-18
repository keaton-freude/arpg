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

!win32
{
    INCLUDEPATH += /usr/local/include/OGRE
}

win32
{
    INCLUDEPATH += ..\..\..\..\..\OgreSDK\ogre\build\sdk\include\OGRE
}

include(QOgreColorPickerWidget.pri)

FORMS += \
    QOgreColorPickerWidget.ui


win32
{
    LIBS += -L"..\..\..\..\..\OgreSDK\ogre\build\sdk\lib\debug" -lOgreMain_d -lOgreHlmsUnlit_d -lOgreHlmsPbs_d
}
