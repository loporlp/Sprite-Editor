#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"
#include <QPushButton>
#include <QImage>
#include <QLayout>
#include <QObject>
#include <QLabel>
#include <QColor>
#include <QApplication>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <iostream>

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , windowModel(model)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // get the default frame from the model's frame container for use in initialization of the drawing canvas.
    QImage image = windowModel.getFrames().get(windowModel.getCanvasSettings().getCurrentFrameIndex());

    // Replace imageHolder height / width with zoom factor, when implementation begins.
    ui->imageHolder->setPixmap(QPixmap::fromImage(image.scaled(ui->imageHolder->width(), ui->imageHolder->height(), Qt::KeepAspectRatio)));
    ui->imageHolder->setAlignment(Qt::AlignCenter);

    // initialize the layout and add the canvas to the layout
    auto layout = new QVBoxLayout();
    layout->addWidget(ui->imageHolder);
    ui->canvas->setLayout(layout);

    // stylize the canvas within the layout
    ui->canvas->setLineWidth(3);
    ui->canvas->setStyleSheet("QFrame {background-color: rgb(200, 255, 255);"
                              "border-width: 1;"
                              "border-radius: 3;"
                              "border-style: solid;"
                              "border-color: rgb(10, 10, 10)}");
    }

void MainWindow::mousePressEvent(QMouseEvent *event)
    {

    event->accept();

    drawOnEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();

    drawOnEvent(event);
}

void MainWindow::drawOnEvent(QMouseEvent *event) {


    // get x,y coordinates of the image holder label
    int xval = ui->imageHolder->mapFromGlobal(ui->imageHolder->cursor().pos()).x();
    int yval = ui->imageHolder->mapFromGlobal(ui->imageHolder->cursor().pos()).y();

    // scale the x,y coordinates relative to the zoom.
    // NOTE: currently assumes zoom is the imageholder width/height - change to zoom factor when implemented
    int xval_zoom = xval / (ui->imageHolder->width() / 100);
    int yval_zoom = yval / (ui->imageHolder->width() / 100);

    // check to ascertain the cursor is within the QImage holder widget.
    if((xval > 0 && yval > 0) && (xval < ui->imageHolder->width() && yval < ui->imageHolder->height())) {

        uint currentFrame = windowModel.getCanvasSettings().getCurrentFrameIndex();
        QImage &image = windowModel.getFrames().get(currentFrame);

        // Below is part of the "Proof of concept". For the actual implementation, we will need
        // to add a check to whichever color(s) the user has selected, passing those instead of these hardcoded colors.
        if(event->buttons() == Qt::RightButton) {
            windowModel.getFrames().setFramePixel(image, xval_zoom, yval_zoom, qRgb(255, 0, 0));
        } else if (event->buttons() == Qt::LeftButton) {
            windowModel.getFrames().setFramePixel(image, xval_zoom, yval_zoom, qRgb(0, 0, 255));
        } else {
            windowModel.getFrames().setFramePixel(image, xval_zoom, yval_zoom, qRgb(0, 255, 0));
        }

        // this updates the label pixmap and scales it. It is currently scaled to the size of the image Holder
        ui->imageHolder->setPixmap(QPixmap::fromImage(image.scaled(ui->imageHolder->width(), ui->imageHolder->height(), Qt::KeepAspectRatio)));

        // DEBUGGING:
        // qDebug() << "NO ZOOM: " << "(" << xval << "," << yval << ")";
        // qDebug() << "ZOOM: " << "(" << xval_zoom << "," << yval_zoom << ")";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    }

