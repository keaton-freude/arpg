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

QString QOgreColorPickerWidget::getColorPickerTitle()
{
    return this->colorPickerTitle;
}

void QOgreColorPickerWidget::setColorPickerTitle(QString s)
{
    this->colorPickerTitle = s;
}
