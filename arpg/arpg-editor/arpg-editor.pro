#-------------------------------------------------
#
# Project created by QtCreator 2017-03-19T20:18:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = arpg-editor
TEMPLATE = app
QT += designer

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -std=c++14

!win32
{
    INCLUDEPATH += /usr/local/include/OGRE
}

win32
{
    INCLUDEPATH += ..\..\..\OgreSDK\ogre\build\sdk\include\OGRE
}

SOURCES += main.cpp\
        MainWindow.cpp \
    QTOgreWindow.cpp \
    OgreLogWatcher.cpp \
    RenderSystem.cpp \
    ArpgCamera.cpp \
    LightDialog.cpp \
    DirectionalLight.cpp

HEADERS  += MainWindow.h \
    QTOgreWindow.h \
    OgreLogWatcher.h \
    RenderSystem.h \
    ArpgCamera.h \
    LightDialog.h \
    DirectionalLightSettings.h \
    DirectionalLight.h \
    AmbientLight.h


# Plugins headers..

HEADERS += ./PluginProjects/QOgreColorPickerPlugin/QOgreColorPickerWidget.h
SOURCES += ./PluginProjects/QOgreColorPickerPlugin/QOgreColorPickerWidget.cpp

INCLUDEPATH += ./PluginProjects/QOgreColorPickerPlugin
INCLUDEPATH += ./PluginProjects/build_colorPickerPlugin

FORMS    += MainWindow.ui \
    LightDialog.ui

!win32
{
    LIBS += -L"/usr/local/lib" -lOgreMain -lOgreHlmsUnlit -lOgreHlmsPbs
}

win32
{
    LIBS += -L"..\..\..\OgreSDK\ogre\build\sdk\lib\debug" -lOgreMain_d -lOgreHlmsUnlit_d -lOgreHlmsPbs_d
}




