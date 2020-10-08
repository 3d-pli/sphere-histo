#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->sphereWidget = ui->sphereWidget;

    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(changeTriangleDepth(int)));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this , SLOT(changeColorMap(QString)));
    connect(ui->checkBoxShowPoints, SIGNAL(stateChanged(int)), this , SLOT(changeShowPoints(int)));
    connect(ui->checkBoxShowSphere, SIGNAL(stateChanged(int)), this , SLOT(changeShowSphere(int)));

    RenderData::getInstance()->setColorMap("Viridis");
//    if(QCoreApplication::arguments().size() > 1){
//        std::string filename = QCoreApplication::arguments().at(1).toStdString();
//        RenderData::getInstance()->loadPointsFromFile(filename);
//        updatePoints();
//        updateTriangleColor();
//    }
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

void MainWindow::changeShowPoints(int showPoints)
{
    RenderData::getInstance()->setPointsSelected(!!showPoints);
    ui->sphereWidget->update();
}

void MainWindow::changeShowSphere(int showSphere)
{
    RenderData::getInstance()->setIcosphereSelected(!!showSphere);
    ui->sphereWidget->update();
}


