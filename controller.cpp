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
    undoConnections();
    setupDrawConnections();

}

void Controller::undoConnections()
{
    Canvas *canvas = view.canvas();
    // MainWindow *view = view;
    connect(canvas, &Canvas::canvasMousePressed, this, [this]() { model.addUndoStack(&currentImage); });
    connect(&view, &MainWindow::undoAction, this, [this]() { model.undo(); });
    connect(&model, &Model::updateCanvas, this, [this](QImage image) { view.canvas()->setImage(&image); });
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
