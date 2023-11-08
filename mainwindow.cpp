#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QImage>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPushButton *btn = new QPushButton();
    auto layout = new QVBoxLayout();
    layout->addWidget(btn);
    ui->canvas->setLayout(layout);

    ui->canvas->setLineWidth(3);

}

MainWindow::~MainWindow()
{
    delete ui;
}

