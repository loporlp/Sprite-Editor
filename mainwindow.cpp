#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

Canvas *MainWindow::canvas()
{
    return this->ui->canvas;
}

MainWindow::~MainWindow()
{
    delete ui;
}
