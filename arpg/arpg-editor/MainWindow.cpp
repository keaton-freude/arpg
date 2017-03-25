#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QTOgreWindow.h"
#include "FileWatcher.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    FileWatcher watcher("Ogre.log");

    connect(&watcher, &FileWatcher::fileChanged, this, &MainWindow::fileChanged);

    QTOgreWindow* ogreWindow = new QTOgreWindow();
    QWidget* renderingContainer = QWidget::createWindowContainer(ogreWindow);

    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(renderingContainer, tr("New Ogre Window"));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileChanged(string newContents)
{
    /* Write to the UI's log window */
}
