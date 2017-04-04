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
