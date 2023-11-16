/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): David Cosby, Andrew Wilhelm, Allison Walker,
 * Mason Sansom, AJ Kennedy, Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * Controller Header
 *
 * File reviewed by: Brett Baxter, Allison Walker
 *
 * Brief:
 * Controls and sets up connections between
 * model and view items.
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

    /// Current image being manipulated
    QImage currentImage;

    Q_OBJECT

public:
    /// Constructor taking references to the model and view
    Controller(Model &model, MainWindow &view);

    /// Setup connections between model, view, and controller
    void setupConnections();

    /// Setup file management-related connections
    void setupFileManagement();

    /// Setup frame management-related connections
    void setupFrameManagement();

    /// Setup connections related to undo functionality
    void setupUndoConnections();

    /// Setup connections related to animation
    void setupAnimationConnections();
private:
    /// Setup connections related to drawing
    void setupDrawConnections();

signals:
    /// Signal to inform about drawing events
    void drawOnEvent(QImage &image, QPoint pos);
};

#endif // CONTROLLER_H
