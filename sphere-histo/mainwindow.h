#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include "spherewidget.h"
#include "renderdata.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void changeTriangleDepth(int depth);
    void openFile();
    void changeColorMap(QString mapName);
    void changeShowPoints(int showPoints);
    void changeShowSphere(int showSphere);
    void changeMirrorPoints(int mirrorPoints);

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    Ui::MainWindow *ui;
    SphereWidget * sphereWidget;
};

#endif // MAINWINDOW_H
