#pragma once
#include <QWidget>

#include "QTOgreWindow.h"

class QTOgreWidget: public QWidget
{
    Q_OBJECT
public:
    QTOgreWidget()
    {
        QTOgreWindow* ogreWindow = new QTOgreWindow();


    }
};
