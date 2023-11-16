#include "controller.h"
#include "canvas.h"
#include "mainwindow.h"
#include "model.h"
#include <QBuffer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

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
    setupFileManagement();
    setupFrameManagement();

}

void Controller::setupUndoConnections()
{
    Canvas *canvas = view.canvas();
    // MainWindow *view = view;
    connect(canvas, &Canvas::canvasMousePressed, this, [this]() {
        model.addUndoStack(&currentImage);
    });

//    connect(canvas, &Canvas::canvasMouseReleased, this, [this]() {
//        model.updateFrame(&currentImage);
//    });

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

    connect(this, &Controller::drawOnEvent, &model, &Model::recieveDrawOnEvent);

    connect(&view, &MainWindow::setPenColor, &model, &Model::recievePenColor);

    connect(&view, &MainWindow::selectActiveTool, &model, &Model::recieveActiveTool);

    connect(&view, &MainWindow::selectBrushSettings, &model, &Model::recieveBrushSettings);

    connect(&model, &Model::sendColor, &view, &MainWindow::recieveNewColor);

    connect(canvas, &Canvas::canvasMousePressed, this, [this, canvas](QPoint pos) {
        emit drawOnEvent(currentImage, pos);
        canvas->update();
    });

    connect(canvas, &Canvas::canvasMouseMoved, this, [this, canvas](QPoint pos) {
        emit drawOnEvent(currentImage, pos);
        canvas->update();
    });
}

void Controller::setupFileManagement()
{
    connect(&view, &MainWindow::saveFile, this, [this](QString fileDirectory)
    {
                // save the current frame before saving conventions
                model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

                QFile file(fileDirectory);
                QVector<QByteArray> imageDataArray;
                QByteArray bufferArray;
                for(uint i = 0; i < model.getFrames().numFrames(); i++) {
                    QBuffer buffer(&bufferArray);
                    buffer.open(QIODevice::WriteOnly);
                    model.getFrames().get(i).save(&buffer, "PNG");
                    imageDataArray.push_back(bufferArray);
                    bufferArray.clear();
                }
                QJsonArray jsonArray;
                for(int i = 0; i < imageDataArray.size(); i++)
                {
                    QJsonObject imageObject;
                    imageObject.insert("QImage", QString(imageDataArray.at(i).toHex()));
                    imageObject["QImage"] = QString(imageDataArray.at(i).toHex());
                    jsonArray.insert(i, imageObject);
                }
                QJsonDocument jsonDoc;
                jsonDoc.setArray(jsonArray);
                file.open(QIODevice::WriteOnly);
                file.write(jsonDoc.toJson());
                file.close();
    });

    connect(&view, &MainWindow::loadFile, this, [this](QString fileDirectory)
    {
        QFile sessionDataFile(fileDirectory);
        QString jsonString;
        if(sessionDataFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            jsonString = sessionDataFile.readAll();
            sessionDataFile.close();
        }

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonArray imageArray = jsonDoc.array();
        QJsonObject tempObj;
        QByteArray tempByteArray;
        QVector<QByteArray> imageDataArray;
        QImage tempImage;

        model.getFrames().clearFrames();
        for(auto imageData : imageArray) {
            QString temp = imageData.toObject().value("QImage").toString();
            tempObj = imageData.toObject();
            tempByteArray = tempByteArray.fromHex(temp.toLatin1());
            imageDataArray.push_back(tempByteArray);

            tempImage.loadFromData(tempByteArray);
            model.getFrames().push(tempImage);
            tempByteArray.clear();
        }
        model.getCanvasSettings().setCurrentFrameIndex(0);
        currentImage = model.getFrames().get(0);
        view.canvas()->setImage(&currentImage);
        view.addFramesToList(model.getFrames().numFrames()-1);
    });

    connect(&view, &MainWindow::newFile, this, [this]()
    {
        model.getFrames().clearFrames();
        model.getFrames().generateFrame(64, 64);
        model.getCanvasSettings().setCurrentFrameIndex(0);
        currentImage = model.getFrames().get(0);
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

        // clear undo / redo button buffers when changing to a new image
        model.redoBuffer.clear();
        model.undoBuffer.clear();

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

