#include "controller.h"
#include <QBuffer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
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
    setupFileManagement();
    setupFrameManagement();
    setupAnimationConnections();
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
    connect(&view, &MainWindow::saveFile, this, [this](QString fileDirectory) {
        // save the current frame before saving conventions
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        QFile file(fileDirectory);
        QVector<QByteArray> imageDataArray;
        QByteArray bufferArray;

        // for every frame in the model, convert the QImage frame into
        // an array of bytes. Add that array to a vector.
        for (uint i = 0; i < model.getFrames().numFrames(); i++) {
            QBuffer buffer(&bufferArray);
            buffer.open(QIODevice::WriteOnly);
            model.getFrames().get(i).save(&buffer, "PNG");
            imageDataArray.push_back(bufferArray);
            bufferArray.clear();
        }

        // add each byte array, representing QImage data, into a json Array
        QJsonArray jsonArray;
        for (int i = 0; i < imageDataArray.size(); i++) {
            QJsonObject imageObject;
            imageObject.insert("QImage", QString(imageDataArray.at(i).toHex()));
            imageObject["QImage"] = QString(imageDataArray.at(i).toHex());
            jsonArray.insert(i, imageObject);
        }

        // add the json Array into a json Document, adding that document into
        // the new file
        QJsonDocument jsonDoc;
        jsonDoc.setArray(jsonArray);
        file.open(QIODevice::WriteOnly);
        file.write(jsonDoc.toJson());
        file.close();
    });

    connect(&view, &MainWindow::loadFile, this, [this](QString fileDirectory) {
        QFile sessionDataFile(fileDirectory);
        QString jsonString;

        // check if the file is actually opened
        if (sessionDataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            jsonString = sessionDataFile.readAll();
            sessionDataFile.close();
        } else {
            qDebug() << "file could not be opened! Did you select the proper directory?";
            return;
        }

        // initialize java objects necessary for json serialization
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonArray imageArray = jsonDoc.array();
        QJsonObject tempObj;
        QByteArray tempByteArray;
        QImage tempImage;

        // clear out all the current frames before loading new ones
        model.getFrames().clearFrames();

        for (auto imageData : imageArray) {
            // extract the "QImage" hex string data from the json object.
            QString jsonString = imageData.toObject().value("QImage").toString();

            // convert hex back into a byte string
            tempObj = imageData.toObject();
            tempByteArray = tempByteArray.fromHex(jsonString.toLatin1());

            // load the byte string data into the temporary image, add image to frames
            tempImage.loadFromData(tempByteArray);
            model.getFrames().push(tempImage);
            tempByteArray.clear();
        }

        // default the canvas settings, set the current image to the first image in the frames vector,
        // update the canvas to display the first image
        model.getCanvasSettings().setCurrentFrameIndex(0);
        currentImage = model.getFrames().get(0);
        view.canvas()->setImage(&currentImage);
        view.addFramesToList(model.getFrames().numFrames() - 1);
    });

    connect(&view, &MainWindow::newFile, this, [this]() {
        // delete all frames and generate a default 64 x 64 frame.
        model.getFrames().clearFrames();
        model.getFrames().generateFrame(64, 64);

        // Default the current index and image.
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
        model.getCanvasSettings().setCurrentFrameIndex(model.getFrames().numFrames() - 1);

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
        if (currentFrameIndex > 0) {
            model.getCanvasSettings().setCurrentFrameIndex(currentFrameIndex - 1);
        }

        // set the current image and update canvas
        currentImage = model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex());
        view.canvas()->setImage(&currentImage);
    });

    connect(&view, &MainWindow::setFrame, this, [this](int frameIndex) {
        //clear undo and redo buffers when switching frames
        model.clearBuffers();

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

void Controller::setupAnimationConnections()
{
    connect(&view, &MainWindow::setFPS, &model, &Model::updateFPS);

    connect(&view, &MainWindow::startAnimation, &model, &Model::updatePlay);

    connect(&model, &Model::updateAnimationPreview, &view, &MainWindow::receiveAnimationFrameData);
}
