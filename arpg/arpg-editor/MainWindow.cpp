#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QTOgreWindow.h"
#include <OgreLogManager.h>
#include <QColorDialog>
#include <thread>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ogreWindow(nullptr),
    lightDialog(nullptr)
{
    ui->setupUi(this);
    ui->logList->setUniformItemSizes(true);

    ogreWindow = new QTOgreWindow(&logWatcher);

    connect(&logWatcher, &OgreLogWatcher::logAdded, this, &MainWindow::logAdded);

    QWidget* renderingContainer = QWidget::createWindowContainer(ogreWindow);

    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(renderingContainer, tr("New Ogre Window"));

    std::this_thread::sleep_for(std::chrono::duration<float>(1.0f));
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
    if (lightDialog == nullptr)
    {
        lightDialog = new LightDialog(this);
        lightDialog->SetRenderSystem(ogreWindow->GetRenderSystem());
        lightDialog->Initialize();
    }
    lightDialog->show();
}
