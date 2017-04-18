#include "LightDialog.h"
#include "ui_LightDialog.h"
#include <QColorDialog>

LightDialog::LightDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LightDialog),
    startingLowerHemisphere(Ogre::ColourValue( 0.6f, 0.45f, 0.3f ) * 0.065f * 0.75f),
    startingUpperHemisphere(Ogre::ColourValue( 0.3f, 0.5f, 0.7f ) * 0.1f * 0.75f)
{
    ui->setupUi(this);

    //    connect(&logWatcher, &OgreLogWatcher::logAdded, this, &MainWindow::logAdded);
    connect(ui->lowerHemisphereColor, &QOgreColorPickerWidget::colorChanged, this, &LightDialog::lowerHemisphereColorChanged);
    connect(ui->upperHemisphereColor, &QOgreColorPickerWidget::colorChanged, this, &LightDialog::upperHemisphereColorChanged);
    connect(ui->diffuseColor, &QOgreColorPickerWidget::colorChanged, this, &LightDialog::diffuseColorChanged);

}

LightDialog::~LightDialog()
{
    delete ui;
}

void LightDialog::SetRenderSystem(RenderSystem *renderSystem)
{
    this->renderSystem = renderSystem;
}

void LightDialog::Initialize()
{
    ui->lowerHemisphereColor->SetCurrentColor(startingLowerHemisphere);
    ui->upperHemisphereColor->SetCurrentColor(startingUpperHemisphere);
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

void LightDialog::on_btnSelectDiffuseColor_clicked()
{
    auto color = QColorDialog::getColor(Qt::white, this, "Pick color", QColorDialog::DontUseNativeDialog);

    if (color.isValid())
    {
        // Color selected!
        // Convert to an Ogre ColourValue, then set it
        Ogre::ColourValue ogreColor(color.redF(), color.greenF(), color.blueF());
        renderSystem->GetDirectionalLight()->SetDiffuseColor(ogreColor);
    }
}

void LightDialog::on_chkAmbientLightEnabled_clicked(bool checked)
{

}

AmbientLight LightDialog::GetSettingsFromUi()
{
    AmbientLight settings;

    auto lowerHemisphereColor = ui->lowerHemisphereColor->GetCurrentColor();
    auto upperHemisphereColor = ui->upperHemisphereColor->GetCurrentColor();

    Ogre::Vector3 hemisphereDirection;
    hemisphereDirection.x = ui->txtAmbientHemisphereDirX->text().toFloat();
    hemisphereDirection.y = ui->txtAmbientHemisphereDirY->text().toFloat();
    hemisphereDirection.z = ui->txtAmbientHemisphereDirZ->text().toFloat();

    settings._envmapScale = 1.0f;
    settings._lowerHemisphere = lowerHemisphereColor;
    settings._upperHemisphere = upperHemisphereColor;
    settings._hemisphereDirection = hemisphereDirection;

    settings._hemisphereDirection = -renderSystem->GetDirectionalLight()->GetDirection() + Ogre::Vector3::UNIT_Y * 0.2f;

    return settings;
}

Ogre::ColourValue LightDialog::GetColourValueFromWidget(QWidget *widget, QColor initialColor)
{
    auto color = QColorDialog::getColor(initialColor, this, "Pick color", QColorDialog::DontUseNativeDialog);

    if (color.isValid())
    {
        // Convert to an Ogre ColourValue
        Ogre::ColourValue ogreColor(color.redF(), color.greenF(), color.blueF());
        return ogreColor;
    }

    return Ogre::ColourValue::White;
}

QColor LightDialog::GetQColorFromWidget(QWidget *widget)
{
    return QColor();
}

void LightDialog::on_chkAmbientLightEnabled_toggled(bool checked)
{
    if (checked)
    {
        renderSystem->SetAmbientSettings(GetSettingsFromUi());
    }
    else
    {
        renderSystem->DisableAmbientLight();
    }
}

void LightDialog::lowerHemisphereColorChanged(Ogre::ColourValue color)
{
    renderSystem->SetAmbientSettings(GetSettingsFromUi());
}

void LightDialog::upperHemisphereColorChanged(Ogre::ColourValue color)
{
    renderSystem->SetAmbientSettings(GetSettingsFromUi());
}

void LightDialog::diffuseColorChanged(Ogre::ColourValue color)
{
    renderSystem->GetDirectionalLight()->SetDiffuseColor(color);
}
