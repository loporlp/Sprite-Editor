#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QImage>
#include <QLayout>
#include <QObject>
#include <QLabel>
#include <QColor>
#include <QApplication>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage image(100, 100, QImage::Format_ARGB32);

    image.fill(QColor(Qt::black));
    image.setPixelColor(50, 50, QColor(Qt::white));
    //Replace with test image to see, I didn't want to add images to repo
    //image->load("C:\\Users\\mason\\OneDrive\\Pictures\\Screenshots\\Screenshot 2023-03-01 231609.png");

    QLabel *imageHolder = new QLabel("");
    imageHolder->setPixmap(QPixmap::fromImage(image.scaled(100, 100, Qt::KeepAspectRatio)));
    auto layout = new QVBoxLayout();
    layout->addWidget(imageHolder);
    ui->canvas->setLayout(layout);

    ui->canvas->setLineWidth(3);
    ui->canvas->setStyleSheet("QFrame {background-color: rgb(200, 255, 255);"
                              "border-width: 1;"
                              "border-radius: 3;"
                              "border-style: solid;"
                              "border-color: rgb(10, 10, 10)}");

    image.fill(QColor(Qt::black));

    imageHolder->setPixmap(QPixmap::fromImage(image.scaled(100, 100, Qt::KeepAspectRatio)));
    QTimer::singleShot(1000, this, SLOT(image.fill(QColor(Qt::white))));

}

MainWindow::~MainWindow()
{
    delete ui;
}

