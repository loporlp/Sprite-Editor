#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "mainwindow.h"
#include "model.h"

class Controller : QObject
{
    Model &model;
    MainWindow &view;
    QImage currentImage;

    Q_OBJECT

public:
    Controller(Model &model, MainWindow &view);
    void setupConnections();

private:
    void setupDrawConnections();
    void setupAnimationConnections();
};

#endif // CONTROLLER_H
