#include "LightDialog.h"
#include "ui_LightDialog.h"

LightDialog::LightDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LightDialog)
{
    ui->setupUi(this);
}

LightDialog::~LightDialog()
{
    delete ui;
}
