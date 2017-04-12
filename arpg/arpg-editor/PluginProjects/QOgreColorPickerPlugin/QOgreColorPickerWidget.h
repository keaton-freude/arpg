#ifndef QOGRECOLORPICKERWIDGET_H
#define QOGRECOLORPICKERWIDGET_H

#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>

namespace Ui {
class QOgreColorPickerWidget;
}

class QDESIGNER_WIDGET_EXPORT QOgreColorPickerWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString colorPickerTitle READ colorPickerTitle WRITE setColorPickerTitle NOTIFY titleChanged DESIGNABLE true)
public:
    explicit QOgreColorPickerWidget(QWidget *parent = 0);
    ~QOgreColorPickerWidget();

public slots:
    QString colorPickerTitle() const
    {
        return m_colorPickerTitle;
    }

    void setColorPickerTitle(QString s);


protected:
    QString m_colorPickerTitle;

signals:
    void titleChanged(QString s);

private:
    Ui::QOgreColorPickerWidget *ui;
};

#endif // QOGRECOLORPICKERWIDGET_H
