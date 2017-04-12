#include "QOgreColorPickerWidget.h"
#include "ui_QOgreColorPickerWidget.h"

QOgreColorPickerWidget::QOgreColorPickerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QOgreColorPickerWidget)
{
    ui->setupUi(this);
}

QOgreColorPickerWidget::~QOgreColorPickerWidget()
{
    delete ui;
}

void QOgreColorPickerWidget::setColorPickerTitle(QString s)
{
    m_colorPickerTitle = s;
    if (ui)
        ui->label->setText(s);
    emit titleChanged(s);
}
