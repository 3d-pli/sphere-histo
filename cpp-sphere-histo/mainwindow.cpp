#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(changeTriangleDepth(int)));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionPlasma, SIGNAL(triggered()), this , SLOT(changeColorMap("actionPlasma")));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeTriangleDepth(int depth)
{
    ui->sphereWidget->setTriangleDepth(depth);
}

void MainWindow::openFile()
{
    QString file = QFileDialog::getOpenFileName(this, "", "", "*.npy;;*.*");

    if(file.isEmpty())
        return;

    ui->sphereWidget->openFile(file.toStdString());
}

void MainWindow::changeColorMap(std::string mapName){
    ui->sphereWidget->setColorMap(mapName);
}
