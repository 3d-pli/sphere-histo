#include "spherewidget.h"
#include <QApplication>
#include <QMainWindow>
#include <QSlider>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SphereWidget w;
    QMainWindow mainWindow;



    QSlider q(Qt::Horizontal, &mainWindow);
    q.setMinimum(0);
    q.setMaximum(8);
    q.setTickInterval(1);

    mainWindow.setCentralWidget(&w);
    mainWindow.setMenuWidget(&q);

    mainWindow.resize(QSize(800, 820));
    mainWindow.show();

    return a.exec();
}
