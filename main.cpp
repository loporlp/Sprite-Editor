#include "controller.h"
#include "mainwindow.h"
#include "model.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model m;
    MainWindow w;
    Controller c(m, w);
    w.show();
    return a.exec();
}
