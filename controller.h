#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "mainwindow.h"
#include "model.h"

class Controller
{
    Model &model;
    MainWindow &view;

public:
    Controller(Model &model, MainWindow &view);
};

#endif // CONTROLLER_H
