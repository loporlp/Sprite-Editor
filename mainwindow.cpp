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
#include <iostream>

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , windowModel(model)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);

    QImage image = windowModel.getFrames().get(0);

    // this is a zoom to help fit the frame to the image holder label.
    ui->imageHolder->setPixmap(QPixmap::fromImage(image.scaled(ui->imageHolder->width(), ui->imageHolder->height(), Qt::KeepAspectRatio)));
    ui->imageHolder->setAlignment(Qt::AlignCenter);
    auto layout = new QVBoxLayout();
    layout->addWidget(ui->imageHolder);
    ui->canvas->setLayout(layout);

    ui->canvas->setLineWidth(3);
    ui->canvas->setStyleSheet("QFrame {background-color: rgb(200, 255, 255);"
                              "border-width: 1;"
                              "border-radius: 3;"
                              "border-style: solid;"
                              "border-color: rgb(10, 10, 10)}");

    QTimer::singleShot(1000, this, SLOT(image.fill(QColor(Qt::white))));

    }

void MainWindow::mousePressEvent(QMouseEvent *event)
    {

    event->accept();

    drawOnEvent();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    event->accept();

    drawOnEvent();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // TODO - correctly update the pixel map
}

void MainWindow::drawOnEvent() {

    int xval = ui->imageHolder->mapFromGlobal(ui->imageHolder->cursor().pos()).x();
    int yval = ui->imageHolder->mapFromGlobal(ui->imageHolder->cursor().pos()).y();
    int xval_zoom = xval / (ui->imageHolder->width() / 100);
    int yval_zoom = yval / (ui->imageHolder->width() / 100);

    if((xval > 0 && yval > 0) && (xval < ui->imageHolder->width() && yval < ui->imageHolder->height())) {
        QImage image = windowModel.getFrames().get(0);
        windowModel.getFrames().drawTest(image, xval_zoom, yval_zoom, QColor(Qt::white));
        ui->imageHolder->setPixmap(QPixmap::fromImage(image.scaled(ui->imageHolder->width(), ui->imageHolder->height(), Qt::KeepAspectRatio)));
        qDebug() << "(" << xval << "," << yval << ")";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    }

