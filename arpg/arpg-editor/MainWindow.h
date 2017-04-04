#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <OgreLogWatcher.h>
#include "LightDialog.h"

using std::string;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void logAdded(string message);

    void on_actionLights_triggered();

private:
    Ui::MainWindow *ui;
    LightDialog* lightDialog;
    OgreLogWatcher logWatcher;
};

#endif // MAINWINDOW_H
