/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): -----------
 * Course: CS 3505
 * Fall 2023
 *
 * Controller Header
 *
 * Brief:
 *
 *
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "mainwindow.h"
#include "model.h"

class Controller : public QObject
{
    Model &model;
    MainWindow &view;
    QImage currentImage;

    Q_OBJECT

public:
    Controller(Model &model, MainWindow &view);
    void setupConnections();
    void setupFileManagement();
    void setupFrameManagement();
    void setupUndoConnections();
    void setupAnimationConnections();
private:
    void setupDrawConnections();

signals:
    void drawOnEvent(QImage &image, QPoint pos);
};

#endif // CONTROLLER_H
