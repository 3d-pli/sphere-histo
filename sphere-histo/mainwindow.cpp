#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    renderData(RenderData::getInstance())
{
    ui->setupUi(this);
    this->sphereWidget = ui->sphereWidget;
    this->colorBarWidget = ui->colorBarWidget;

    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(changeTriangleDepth(int)));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this , SLOT(changeColorMap(QString)));
    connect(ui->checkBoxShowPoints, SIGNAL(stateChanged(int)), this , SLOT(changeShowPoints(int)));
    connect(ui->checkBoxShowSphere, SIGNAL(stateChanged(int)), this , SLOT(changeShowSphere(int)));
    connect(ui->checkBoxMirrorPoints, SIGNAL(stateChanged(int)), SLOT(changeMirrorPoints(int)));

    this->renderData->setColorMap("Viridis");
    this->colorBarWidget->update();
//    this->colormapGraphicsView->setColormap("Viridis");

}

MainWindow::~MainWindow()
{
    delete sphereWidget;
    delete ui;
}

void MainWindow::changeTriangleDepth(int depth)
{
    this->renderData->setSphereDepth(depth);
    ui->labelMaxPointsPerTriangle->setText(QString::number(renderData->getMaxPointsPerTriangle()));
    ui->sphereWidget->updateSphereVertices();
}

void MainWindow::openFile()
{
    QString file = QFileDialog::getOpenFileName(this, "", "", "*.npy;;*.*");

    if(file.isEmpty())
        return;
    std::string filename = file.toStdString();
    this->renderData->loadPointsFromFile(filename);
    ui->labelMaxPointsPerTriangle->setText(QString::number(renderData->getMaxPointsPerTriangle()));
    ui->sphereWidget->updatePoints();
    ui->sphereWidget->updateTriangleColor();
}

void MainWindow::changeColorMap(QString mapName){
    this->renderData->setColorMap(mapName);
    this->sphereWidget->updateTriangleColor();
    this->colorBarWidget->update();
}

void MainWindow::changeShowPoints(int showPoints)
{
    this->renderData->setPointsSelected(!!showPoints);
    ui->sphereWidget->update();
}

void MainWindow::changeShowSphere(int showSphere)
{
    this->renderData->setIcosphereSelected(!!showSphere);
    ui->sphereWidget->update();
}

void MainWindow::changeMirrorPoints(int mirrorPoints)
{
    this->renderData->setMirrorPointsAndRecalculate(!!mirrorPoints);
    ui->labelMaxPointsPerTriangle->setText(QString::number(renderData->getMaxPointsPerTriangle()));
    ui->sphereWidget->updatePoints();
    ui->sphereWidget->updateTriangleColor();
}


