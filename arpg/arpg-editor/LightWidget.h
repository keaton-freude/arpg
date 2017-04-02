#ifndef LIGHTWIDGET_H
#define LIGHTWIDGET_H

#include <QDockWidget>

namespace Ui {
class LightWidget;
}

class LightWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit LightWidget(QWidget *parent = 0);
    ~LightWidget();

private:
    Ui::LightWidget *ui;
};

#endif // LIGHTWIDGET_H
