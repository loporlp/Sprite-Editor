#include <QApplication>

#include "controller.h"
#include "mainwindow.h"
#include "model.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model m;
    MainWindow w;
    w.show();
    Controller c(m, w);
    return a.exec();
}
