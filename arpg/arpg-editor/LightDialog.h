#ifndef LIGHTDIALOG_H
#define LIGHTDIALOG_H

#include <QDialog>
#include "RenderSystem.h"

namespace Ui {
class LightDialog;
}

class LightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LightDialog(QWidget *parent = 0);
    ~LightDialog();

    void SetRenderSystem(RenderSystem* renderSystem);
    void Initialize();
private slots:
    void on_chkDirectionalLightEnabled_stateChanged(int arg1);

    void on_chkDirectionalLightEnabled_clicked(bool checked);

    void on_sliderDirLightX_sliderMoved(int position);

    void on_sliderDirLightY_sliderMoved(int position);

    void on_sliderDirLightZ_sliderMoved(int position);

    void on_btnSelectDiffuseColor_clicked();

    void on_chkAmbientLightEnabled_clicked(bool checked);

    void on_chkAmbientLightEnabled_toggled(bool checked);

private slots:
    void lowerHemisphereColorChanged(Ogre::ColourValue color);
    void upperHemisphereColorChanged(Ogre::ColourValue color);
    void diffuseColorChanged(Ogre::ColourValue color);

private:
    Ui::LightDialog *ui;
    RenderSystem* renderSystem;

    AmbientLight GetSettingsFromUi();
    Ogre::ColourValue GetColourValueFromWidget(QWidget* widget, QColor initialColor = Qt::white);
    QColor GetQColorFromWidget(QWidget* widget);
    Ogre::ColourValue startingLowerHemisphere;
    Ogre::ColourValue startingUpperHemisphere;
};

#endif // LIGHTDIALOG_H
