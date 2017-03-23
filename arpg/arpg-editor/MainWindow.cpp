#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QTOgreWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
