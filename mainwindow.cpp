#include "mainwindow.h"
#include <QApplication>
#include <QColor>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // get the default frame from the model's frame container for use in initialization of the drawing canvas.
    //    QImage image = windowModel.getFrames().get(
    //        windowModel.getCanvasSettings().getCurrentFrameIndex());

    // Replace imageHolder height / width with zoom factor, when implementation begins.
    //    ui->oldImageHolder->setPixmap(QPixmap::fromImage(image.scaled(ui->oldImageHolder->width(),
    //                                                                  ui->oldImageHolder->height(),
    //                                                                  Qt::KeepAspectRatio)));
    //    ui->oldImageHolder->setAlignment(Qt::AlignCenter);

    //    // initialize the layout and add the canvas to the layout
    //    auto layout = new QVBoxLayout();
    //    layout->addWidget(ui->oldImageHolder);
    //    ui->old_canvas->setLayout(layout);

    //    // stylize the canvas within the layout
    //    ui->old_canvas->setLineWidth(3);
    //    ui->old_canvas->setStyleSheet("QFrame {background-color: rgb(200, 255, 255);"
    //                                  "border-width: 1;"
    //                                  "border-radius: 3;"
    //                                  "border-style: solid;"
    //                                  "border-color: rgb(10, 10, 10)}");
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

void MainWindow::drawOnEvent(QMouseEvent *event)
{
    // get x,y coordinates of the image holder label
    int xval = ui->oldImageHolder->mapFromGlobal(ui->oldImageHolder->cursor().pos()).x();
    int yval = ui->oldImageHolder->mapFromGlobal(ui->oldImageHolder->cursor().pos()).y();

    // scale the x,y coordinates relative to the zoom.
    // NOTE: currently assumes zoom is the imageholder width/height - change to zoom factor when implemented
    int xval_zoom = xval / (ui->oldImageHolder->width() / 100);
    int yval_zoom = yval / (ui->oldImageHolder->width() / 100);

    // check to ascertain the cursor is within the QImage holder widget.
    //    if ((xval > 0 && yval > 0)
    //        && (xval < ui->oldImageHolder->width() && yval < ui->oldImageHolder->height())) {
    //        uint currentFrame = windowModel.getCanvasSettings().getCurrentFrameIndex();
    //        QImage &image = windowModel.getFrames().get(currentFrame);

    //        // Below is part of the "Proof of concept". For the actual implementation, we will need
    //        // to add a check to whichever color(s) the user has selected, passing those instead of these hardcoded colors.
    //        if (event->buttons() == Qt::RightButton) {
    //            windowModel.getFrames().setFramePixel(image, xval_zoom, yval_zoom, qRgb(255, 0, 0));
    //        } else if (event->buttons() == Qt::LeftButton) {
    //            windowModel.getFrames().setFramePixel(image, xval_zoom, yval_zoom, qRgb(0, 0, 255));
    //        } else {
    //            windowModel.getFrames().setFramePixel(image, xval_zoom, yval_zoom, qRgb(0, 255, 0));
    //        }

    //        // this updates the label pixmap and scales it. It is currently scaled to the size of the image Holder
    //        ui->oldImageHolder->setPixmap(QPixmap::fromImage(image.scaled(ui->oldImageHolder->width(),
    //                                                                      ui->oldImageHolder->height(),
    //                                                                      Qt::KeepAspectRatio)));

    // DEBUGGING:
    // qDebug() << "NO ZOOM: " << "(" << xval << "," << yval << ")";
    // qDebug() << "ZOOM: " << "(" << xval_zoom << "," << yval_zoom << ")";
    //}
}

MainWindow::~MainWindow()
{
    delete ui;
}
