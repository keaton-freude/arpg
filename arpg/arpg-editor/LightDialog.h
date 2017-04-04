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

private slots:
    void on_chkDirectionalLightEnabled_stateChanged(int arg1);

    void on_chkDirectionalLightEnabled_clicked(bool checked);

private:
    Ui::LightDialog *ui;
    RenderSystem* renderSystem;
};

#endif // LIGHTDIALOG_H
