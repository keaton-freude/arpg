#include "QOgreColorPickerWidget.h"
#include "QOgreColorPickerWidgetPlugin.h"

#include <QtPlugin>

QOgreColorPickerWidgetPlugin::QOgreColorPickerWidgetPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QOgreColorPickerWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QOgreColorPickerWidgetPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *QOgreColorPickerWidgetPlugin::createWidget(QWidget *parent)
{
    return new QOgreColorPickerWidget(parent);
}

QString QOgreColorPickerWidgetPlugin::name() const
{
    return QLatin1String("QOgreColorPickerWidget");
}

QString QOgreColorPickerWidgetPlugin::group() const
{
    return QLatin1String("Custom Ogre");
}

QIcon QOgreColorPickerWidgetPlugin::icon() const
{
    return QIcon();
}

QString QOgreColorPickerWidgetPlugin::toolTip() const
{
    return QLatin1String("Picks a color, remembers selection and converts to ColourValue.");
}

QString QOgreColorPickerWidgetPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QOgreColorPickerWidgetPlugin::isContainer() const
{
    return false;
}

QString QOgreColorPickerWidgetPlugin::domXml() const
{
    //return QLatin1String("<widget class=\"QOgreColorPickerWidget\" name=\"qOgreColorPickerWidget\">\n</widget>\n");
    return "<ui language=\"c++\">\n"
            " <widget class=\"QOgreColorPickerWidget\" name=\"qOgreColorPickerWidget\">\n"
            "  <property name=\"colorPickerTitle\">\n"
            "   <string>TestString</string>\n"
            "  </property>\n"
            " </widget>\n"
            "</ui>\n";
}

QString QOgreColorPickerWidgetPlugin::includeFile() const
{
    return QLatin1String("QOgreColorPickerWidget.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(QOgreColorPickerWidgetPlugin, QOgreColorPickerWidgetPlugin)
#endif // QT_VERSION < 0x050000
