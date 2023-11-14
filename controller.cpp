#include "controller.h"
#include "canvas.h"
#include "mainwindow.h"
#include "model.h"

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
    setupAnimationConnections();
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

void Controller::setupAnimationConnections()
{
    connect(&view, &MainWindow::setFPS, &model, &Model::updateFPS);

    connect(&view, &MainWindow::startAnimation, &model, &Model::play);

    connect(&view, &MainWindow::startAnimation, this, [this] () {

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, &model, QOverload<>::of(&Model::playAnimationFrames));

        while(model.play)
        {
            timer->start(1000);
        }

    });


    connect(&model, &Model::updateAnimationPreview, this, [this](QImage frame, int delay) {

        QTimer::singleShot(delay, this, [this, frame]() {

            view.playAnimation(frame);
        });

    });

    //    void setFPS(int fps);
    //      TODO: store the fps value which will determine how many frames we loop through per second
}
