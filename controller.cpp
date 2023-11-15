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
    setupUndoConnections();
    setupDrawConnections();

}

void Controller::setupUndoConnections()
{
    Canvas *canvas = view.canvas();
    // MainWindow *view = view;
    connect(canvas, &Canvas::canvasMousePressed, this, [this]() {
        model.addUndoStack(&currentImage);
    });

    connect(canvas, &Canvas::canvasMouseReleased, this, [this]() {
        model.updateFrame(&currentImage);
    });

    connect(&view, &MainWindow::undoAction, this, [this]() { model.undo(); });
    connect(&view, &MainWindow::redoAction, this, [this]() { model.redo(); });

    connect(&model, &Model::updateCanvas, this, [this](QImage image) {
        currentImage = image;
        view.canvas()->setImage(&currentImage);
    });
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
