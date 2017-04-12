#ifndef QOGRECOLORPICKERWIDGET_H
#define QOGRECOLORPICKERWIDGET_H

#include <QtDesigner/QtDesigner>
#include <QWidget>

namespace Ui {
class QOgreColorPickerWidget;
}

class QDESIGNER_WIDGET_EXPORT QOgreColorPickerWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString colorPickerTitle READ getColorPickerTitle WRITE setColorPickerTitle)
public:
    explicit QOgreColorPickerWidget(QWidget *parent = 0);
    ~QOgreColorPickerWidget();

public slots:
    QString getColorPickerTitle();
    void setColorPickerTitle(QString s);

protected:
    QString colorPickerTitle;

private:
    Ui::QOgreColorPickerWidget *ui;

signals:
    void titleChanged(QString title);
};

#endif // QOGRECOLORPICKERWIDGET_H
