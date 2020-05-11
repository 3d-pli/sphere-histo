#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "resources/colormaps_matplotlib.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void changeTriangleDepth(int depth);
    void openFile();
    void changeColorMap(std::string mapName);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
