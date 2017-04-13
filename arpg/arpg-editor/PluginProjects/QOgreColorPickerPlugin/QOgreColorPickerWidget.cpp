#include "QOgreColorPickerWidget.h"
#include "ui_QOgreColorPickerWidget.h"
#include <OgreLogManager.h>
QOgreColorPickerWidget::QOgreColorPickerWidget(QWidget *parent) :
    QWidget(parent),
    currentColor(Ogre::ColourValue::White),
    ui(new Ui::QOgreColorPickerWidget)
{
    ui->setupUi(this);

    SetWidgetBackgroundColor(ui->widget, GetQColorFromOgreColor(currentColor));

}

QOgreColorPickerWidget::~QOgreColorPickerWidget()
{
    delete ui;
}

void QOgreColorPickerWidget::SetCurrentColor(Ogre::ColourValue color)
{
    currentColor = color;
        SetWidgetBackgroundColor(ui->widget, GetQColorFromOgreColor(color));
    emit(colorChanged(currentColor));
}

void QOgreColorPickerWidget::setColorPickerTitle(QString s)
{
    m_colorPickerTitle = s;
    if (ui)
        ui->label->setText(s);
    emit titleChanged(s);
}

void QOgreColorPickerWidget::on_pushButton_clicked()
{
    auto color = QColorDialog::getColor(Qt::white, this, "Pick color", QColorDialog::DontUseNativeDialog);

    if (color.isValid())
    {
        // Color selected!
        // Convert to an Ogre ColourValue, then set it
        Ogre::ColourValue ogreColor(color.redF(), color.greenF(), color.blueF());
        currentColor = ogreColor;

        SetWidgetBackgroundColor(ui->widget, GetQColorFromOgreColor(currentColor));
        emit(colorChanged(currentColor));
    }
}

void QOgreColorPickerWidget::SetWidgetBackgroundColor(QWidget *widget, QColor color)
{
    QPalette palette = widget->palette();
    palette.setColor(QPalette::Background, color);
    widget->setPalette(palette);
}

QColor QOgreColorPickerWidget::GetQColorFromOgreColor(Ogre::ColourValue color)
{
    return QColor(color.r * 255, color.g * 255, color.b * 255);
}
