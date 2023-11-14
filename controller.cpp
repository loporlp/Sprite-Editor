#include "controller.h"
#include "canvas.h"
#include "mainwindow.h"
#include "model.h"
#include <QBuffer>

QByteArray swag;

Controller::Controller(Model &model, MainWindow &view)
    : model(model)
    , view(view)
    , currentImage(model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()))
{
    view.canvas()->setImage(&currentImage);
    setupConnections();
}

void Controller::setupConnections()
{
    setupDrawConnections();
    setupFileManagement();
    setupFrameManagement();
}

void Controller::setupDrawConnections()
{
    Canvas *canvas = view.canvas();

    connect(canvas, &Canvas::canvasMousePressed, this, [this, canvas](QPoint pos) {
        currentImage.setPixelColor(pos.x(), pos.y(), qRgb(0, 0, 0));
        canvas->update();
    });

    connect(canvas, &Canvas::canvasMouseMoved, this, [this, canvas](QPoint pos) {
        currentImage.setPixelColor(pos.x(), pos.y(), qRgb(0, 0, 0));
        canvas->update();
    });
}

void Controller::setupFileManagement()
{
    connect(&view, &MainWindow::saveFile, this, [this](QString fileDirectory) {
        qDebug() << fileDirectory;
        QImage tester;
        for(uint i = 0; i < model.getFrames().numFrames(); i++) {
            tester = model.getFrames().get(i);
        }
        QByteArray ba;
        QBuffer buffer(&swag);
        buffer.open(QIODevice::WriteOnly);
        tester.save(&buffer, "PNG");
        qDebug() << swag;
    });

    connect(&view, &MainWindow::loadFile, this, [this](QString fileDirectory) {
        qDebug() << fileDirectory;
        qDebug() << swag;
        bool testt = currentImage.loadFromData(swag, "PNG");
        qDebug() << testt;
        view.canvas()->setImage(&currentImage);
    });
}

void Controller::setupFrameManagement()
{
    connect(&view, &MainWindow::addFrame, this, [this]() {
        model.getFrames().generateFrame(currentImage.width(), currentImage.height());
        model.getCanvasSettings().setCurrentFrameIndex(model.getFrames().numFrames()-1);
        currentImage = model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex());
        view.canvas()->setImage(&currentImage);
    });

    connect(&view, &MainWindow::deleteFrame, this, [this]() {

        uint currentFrameIndex = model.getCanvasSettings().getCurrentFrameIndex();

        model.getFrames().remove(currentFrameIndex);
        model.getCanvasSettings().setCurrentFrameIndex(currentFrameIndex-1);
        currentImage = model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex());
        view.canvas()->setImage(&currentImage);
    });
}
