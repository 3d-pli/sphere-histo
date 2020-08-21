#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->sphereWidget = ui->sphereWidget;

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(changeTriangleDepth(int)));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this , SLOT(changeColorMap(QString)));

    RenderData::getInstance()->setColorMap("Viridis");
}

MainWindow::~MainWindow()
{
    delete sphereWidget;
    delete ui;
}

void MainWindow::changeTriangleDepth(int depth)
{
    RenderData::getInstance()->setSphereDepth(depth);
    ui->sphereWidget->updateSphereVertices();
}

void MainWindow::openFile()
{
    QString file = QFileDialog::getOpenFileName(this, "", "", "*.npy;;*.*");

    if(file.isEmpty())
        return;
    std::string filename = file.toStdString();
    RenderData::getInstance()->loadPointsFromFile(filename);
    ui->sphereWidget->updatePoints();
    ui->sphereWidget->updateTriangleColor();
}

void MainWindow::changeColorMap(QString mapName){
    RenderData::getInstance()->setColorMap(mapName);
    ui->sphereWidget->updateTriangleColor();
}
