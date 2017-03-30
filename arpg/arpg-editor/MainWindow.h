#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <OgreLogWatcher.h>

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

private:
    Ui::MainWindow *ui;
    OgreLogWatcher logWatcher;
};

#endif // MAINWINDOW_H
