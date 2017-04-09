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

void LightDialog::SetRenderSystem(RenderSystem *renderSystem)
{
    this->renderSystem = renderSystem;
}

void LightDialog::on_chkDirectionalLightEnabled_stateChanged(int arg1)
{
}

void LightDialog::on_chkDirectionalLightEnabled_clicked(bool checked)
{
    // When the box is checked, enable the directional light
    // when its unchecked, disable it.

    if (checked)
        renderSystem->GetDirectionalLight()->Enable();
    else
        renderSystem->GetDirectionalLight()->Disable();
}

void LightDialog::on_sliderDirLightX_sliderMoved(int position)
{
    float value = position / 100.0f;
    QString rounded = QString::number(value, 'f', 2);
    ui->txtDirLightX->setText(rounded);

    // update x component
    renderSystem->GetDirectionalLight()->SetDirectionX(value);
}

void LightDialog::on_sliderDirLightY_sliderMoved(int position)
{
    float value = position / 100.0f;
    QString rounded = QString::number(value, 'f', 2);
    ui->txtDirLightY->setText(rounded);

    // update x component
    renderSystem->GetDirectionalLight()->SetDirectionY(value);
}

void LightDialog::on_sliderDirLightZ_sliderMoved(int position)
{
    float value = position / 100.0f;
    QString rounded = QString::number(value, 'f', 2);
    ui->txtDirLightZ->setText(rounded);

    // update x component
    renderSystem->GetDirectionalLight()->SetDirectionZ(value);
}
