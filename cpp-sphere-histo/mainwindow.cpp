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

    RenderData::getInstance()->setSphereDepth(3);

//    connect(ui->actionPlasma, SIGNAL(triggered()), this , SLOT(changeColorMap()));
    // GEHT SO NICHT: QAction hat nur die Signals triggered() oder hovered() ohne Parameter -> Können nur an Slot ohne Parameter übergeben werden
    // Entweder: Für jede Farbe einen eigenen Slot schreiben -> hässlich
    // ODER: Eine Combobox irgendwo ins UI einbauen
}

MainWindow::~MainWindow()
{
    delete sphereWidget;
    delete ui;
}

SphereWidget * MainWindow::getSphereWidget(){
    return this->sphereWidget;
}

void MainWindow::changeTriangleDepth(int depth)
{
    RenderData::getInstance()->setSphereDepth(depth);
}

void MainWindow::openFile()
{
    QString file = QFileDialog::getOpenFileName(this, "", "", "*.npy;;*.*");

    if(file.isEmpty())
        return;
    RenderData::getInstance()->loadPointsFromFile(file.toStdString());
    ui->sphereWidget->updatePoints();
}

void MainWindow::changeColorMap(QString mapName){
    RenderData::getInstance()->setColorMap(mapName);
}
