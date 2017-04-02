#include "LightWidget.h"
#include "ui_LightWidget.h"

LightWidget::LightWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::LightWidget)
{
    ui->setupUi(this);
}

LightWidget::~LightWidget()
{
    delete ui;
}
