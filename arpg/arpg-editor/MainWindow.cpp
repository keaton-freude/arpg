#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QTOgreWindow.h"
#include <OgreLogManager.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    lightDialog(new LightDialog(this))
{
    ui->setupUi(this);
    ui->logList->setUniformItemSizes(true);

    QTOgreWindow* ogreWindow = new QTOgreWindow(&logWatcher);

    connect(&logWatcher, &OgreLogWatcher::logAdded, this, &MainWindow::logAdded);

    QWidget* renderingContainer = QWidget::createWindowContainer(ogreWindow);

    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(renderingContainer, tr("New Ogre Window"));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logAdded(string message)
{
    ui->logList->addItem(QString::fromStdString(message));
    ui->logList->scrollToBottom();
}

void MainWindow::on_actionLights_triggered()
{
    lightDialog->show();
}
