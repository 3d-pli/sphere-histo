#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "resources/colormaps_matplotlib.h"
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

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SphereWidget * sphereWidget;
};

#endif // MAINWINDOW_H
