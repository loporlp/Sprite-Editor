#include "controller.h"
#include "canvas.h"
#include "mainwindow.h"
#include "model.h"
#include <QBuffer>

Controller::Controller(Model &model, MainWindow &view)
    : model(model)
    , view(view)
{
    currentImage = model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex());
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

    connect(canvas, &Canvas::canvasMousePressed, this, [this, canvas](QPoint pos)
    {
        currentImage.setPixelColor(pos.x(), pos.y(), qRgb(0, 0, 0));
        canvas->update();
    });

    connect(canvas, &Canvas::canvasMouseMoved, this, [this, canvas](QPoint pos)
    {
        currentImage.setPixelColor(pos.x(), pos.y(), qRgb(0, 0, 0));
        canvas->update();
    });
}

void Controller::setupFileManagement()
{
    connect(&view, &MainWindow::saveFile, this, [this](QString fileDirectory)
    {
        // save the current frame before saving conventions
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        QVector<QByteArray> imageDataArray;
        QByteArray ba;
        for(uint i = 0; i < model.getFrames().numFrames(); i++) {
            QBuffer buffer(&ba);
            buffer.open(QIODevice::WriteOnly);
            model.getFrames().get(i).save(&buffer, "PNG");
            imageDataArray.push_back(ba);
            ba.clear();
        }
    });

    connect(&view, &MainWindow::loadFile, this, [this](QString fileDirectory)
    {
// TODO: Implementation from a file.
//        QVector<QByteArray> imageDataArray;
//        bool testt = currentImage.loadFromData(imageDataArray.at(0), "PNG");
//        qDebug() << testt;
        view.canvas()->setImage(&currentImage);
    });
}

void Controller::setupFrameManagement()
{
    connect(&view, &MainWindow::addFrame, this, [this]() {
        // save the current frame
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        // generate a new frame and add to the canvas index.
        model.getFrames().generateFrame(currentImage.width(), currentImage.height());
        model.getCanvasSettings().setCurrentFrameIndex(model.getFrames().numFrames()-1);

        // set the current image and update canvas
        currentImage = model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex());
        view.canvas()->setImage(&currentImage);
    });

    connect(&view, &MainWindow::deleteFrame, this, [this]() {

        uint currentFrameIndex = model.getCanvasSettings().getCurrentFrameIndex();

        // save the current frame
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        model.getFrames().remove(currentFrameIndex);

        // only modify the currentFrameIndex if the current frame is not the first (index 0)
        if(currentFrameIndex > 0) {
            model.getCanvasSettings().setCurrentFrameIndex(currentFrameIndex-1);
        }

        // set the current image and update canvas
        currentImage = model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex());
        view.canvas()->setImage(&currentImage);
    });

    connect(&view, &MainWindow::setFrame, this, [this](int frameIndex) {
        // save the current frame
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        model.getCanvasSettings().setCurrentFrameIndex(frameIndex);

        // set the current image and update canvas
        currentImage = model.getFrames().get(frameIndex);
        view.canvas()->setImage(&currentImage);
    });

    connect(&view, &MainWindow::moveFrame, this, [this](int firstFrame, int secondFrame) {
        // save the current frame
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        // swap frames and set the new current frame index
        model.getFrames().swap(firstFrame, secondFrame);
        model.getCanvasSettings().setCurrentFrameIndex(secondFrame);

        // set the current image and update canvas
        currentImage = model.getFrames().get(secondFrame);
        view.canvas()->setImage(&currentImage);
    });
}

