#ifndef QOGRECOLORPICKERWIDGET_H
#define QOGRECOLORPICKERWIDGET_H

#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>
#include <QColorDialog>

#include <OgreColourValue.h>

namespace Ui {
class QOgreColorPickerWidget;
}

class QOgreColorPickerWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString colorPickerTitle READ colorPickerTitle WRITE setColorPickerTitle NOTIFY titleChanged DESIGNABLE true)
public:
    explicit QOgreColorPickerWidget(QWidget *parent = 0);
    ~QOgreColorPickerWidget();

    void SetCurrentColor(Ogre::ColourValue color);

public slots:
    QString colorPickerTitle() const
    {
        return m_colorPickerTitle;
    }

    void setColorPickerTitle(QString s);

    Ogre::ColourValue GetCurrentColor() const
    {
        return currentColor;
    }

protected:
    QString m_colorPickerTitle;

signals:
    void titleChanged(QString s);
    void colorChanged(Ogre::ColourValue color);

private slots:
    void on_pushButton_clicked();

private:
    Ui::QOgreColorPickerWidget *ui;
    Ogre::ColourValue currentColor;
    void SetWidgetBackgroundColor(QWidget* widget, QColor color);
    QColor GetQColorFromOgreColor(Ogre::ColourValue color);
};

#endif // QOGRECOLORPICKERWIDGET_H
