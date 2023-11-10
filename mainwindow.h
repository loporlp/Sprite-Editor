#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Model& windowModel;
    Q_OBJECT
public:
    MainWindow(Model& model, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    void drawOnEvent();


};
#endif // MAINWINDOW_H
