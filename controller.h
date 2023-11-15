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

private:
    void setupDrawConnections();

    //new from toolGlue
signals:
    void drawOnEvent(QImage &image, QPoint pos);
};

#endif // CONTROLLER_H
