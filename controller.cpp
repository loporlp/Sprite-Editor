/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): David Cosby, Andrew Wilhelm, Allison Walker,
 * Mason Sansom, AJ Kennedy, Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * Controller Source
 *
 * File reviewed by: Brett Baxter, Allison Walker
 *
 * Brief:
 * Controls and sets up connections between
 * model and view items.
 *
 *
*/

#include <QBuffer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>

#include "controller.h"
#include "canvas.h"
#include "mainwindow.h"
#include "model.h"

/**
 * @brief Controller::Controller - Constructor
 * @param model
 * @param view
 */
Controller::Controller(Model &model, MainWindow &view)
    : model(model)
    , view(view)
{
    currentImage = model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex());
    view.canvas()->setImage(&currentImage);
    setupConnections();
}

/**
 * @brief Controller::setupConnections - Sets up various signal-slot connections for the controller
 */
void Controller::setupConnections()
{
    setupUndoConnections();
    setupDrawConnections();
    setupFileManagement();
    setupFrameManagement();
    setupAnimationConnections();
}

/**
 * @brief Controller::setupUndoConnections - Sets up connections related to undo and redo operations
 */
void Controller::setupUndoConnections()
{
    Canvas *canvas = view.canvas();

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

/**
 * @brief Controller::setupDrawConnections - Sets up connections related to drawing on the canvas
 */
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

/**
 * @brief Controller::setupFileManagement - Sets up connections related to file management
 */
void Controller::setupFileManagement()
{
    // Save file connections
    connect(&view, &MainWindow::saveFile, this, [this](QString fileDirectory) {

        // Save the current frame before saving conventions
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        QFile file(fileDirectory);
        QVector<QByteArray> imageDataArray;
        QByteArray bufferArray;

        // For every frame in the model, convert the QImage frame into
        // an array of bytes. Add that array to a vector
        for (uint i = 0; i < model.getFrames().numFrames(); i++) {
            QBuffer buffer(&bufferArray);
            buffer.open(QIODevice::WriteOnly);
            model.getFrames().get(i).save(&buffer, "PNG");
            imageDataArray.push_back(bufferArray);
            bufferArray.clear();
        }

        // Add each byte array, representing QImage data, into a json Array
        QJsonArray jsonArray;
        for (int i = 0; i < imageDataArray.size(); i++) {
            QJsonObject imageObject;
            imageObject.insert("QImage", QString(imageDataArray.at(i).toHex()));
            imageObject["QImage"] = QString(imageDataArray.at(i).toHex());
            jsonArray.insert(i, imageObject);
        }

        // Add the json Array into a json Document, adding that document into
        // the new file
        QJsonDocument jsonDoc;
        jsonDoc.setArray(jsonArray);
        file.open(QIODevice::WriteOnly);
        file.write(jsonDoc.toJson());
        file.close();
    });

    // Load file connections
    connect(&view, &MainWindow::loadFile, this, [this](QString fileDirectory) {
        QFile sessionDataFile(fileDirectory);
        QString jsonString;

        // Check if the file is actually opened
        if (sessionDataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            jsonString = sessionDataFile.readAll();
            sessionDataFile.close();
        } else {
            qDebug() << "file could not be opened! Did you select the proper directory?";
            return;
        }

        // Initialize java objects necessary for json serialization
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonArray imageArray = jsonDoc.array();
        QJsonObject tempObj;
        QByteArray tempByteArray;
        QImage tempImage;

        // Clear out all the current frames before loading new ones
        model.getFrames().clearFrames();

        for (auto imageData : imageArray) {
            // Extract the "QImage" hex string data from the json object
            QString jsonString = imageData.toObject().value("QImage").toString();

            // Convert hex back into a byte string
            tempObj = imageData.toObject();
            tempByteArray = tempByteArray.fromHex(jsonString.toLatin1());

            // Load the byte string data into the temporary image, add image to frames
            tempImage.loadFromData(tempByteArray);
            model.getFrames().push(tempImage);
            tempByteArray.clear();
        }

        // Default the canvas settings, set the current image to the first image in the frames vector,
        // update the canvas to display the first image
        model.getCanvasSettings().setCurrentFrameIndex(0);
        currentImage = model.getFrames().get(0);
        view.canvas()->setImage(&currentImage);
        view.addFramesToList(model.getFrames().numFrames() - 1);
    });

    // New file connections
    connect(&view, &MainWindow::newFile, this, [this]() {
        // Delete all frames and generate a default 64 x 64 frame
        model.getFrames().clearFrames();
        model.getFrames().generateFrame(64, 64);

        // Default the current index and image
        model.getCanvasSettings().setCurrentFrameIndex(0);
        currentImage = model.getFrames().get(0);
        view.canvas()->setImage(&currentImage);
    });
}

/**
 * @brief Controller::setupFrameManagement - Sets up connections related to frame management
 */
void Controller::setupFrameManagement()
{
    connect(&view, &MainWindow::addFrame, this, [this]() {
        // Save the current frame
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        // Generate a new frame and add to the canvas index.
        model.getFrames().generateFrame(currentImage.width(), currentImage.height());
        model.getCanvasSettings().setCurrentFrameIndex(model.getFrames().numFrames() - 1);

        // Set the current image and update canvas
        currentImage = model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex());
        view.canvas()->setImage(&currentImage);
    });

    connect(&view, &MainWindow::deleteFrame, this, [this]() {
        uint currentFrameIndex = model.getCanvasSettings().getCurrentFrameIndex();

        // Save the current frame
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        model.getFrames().remove(currentFrameIndex);

        // Only modify the currentFrameIndex if the current frame is not the first (index 0)
        if (currentFrameIndex > 0) {
            model.getCanvasSettings().setCurrentFrameIndex(currentFrameIndex - 1);
        }

        // Set the current image and update canvas
        currentImage = model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex());
        view.canvas()->setImage(&currentImage);
    });

    connect(&view, &MainWindow::setFrame, this, [this](int frameIndex) {
        //Clear undo and redo buffers when switching frames
        model.clearBuffers();

        // Save the current frame
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        // Clear undo / redo button buffers when changing to a new image
        model.redoBuffer.clear();
        model.undoBuffer.clear();

        model.getCanvasSettings().setCurrentFrameIndex(frameIndex);

        // Set the current image and update canvas
        currentImage = model.getFrames().get(frameIndex);
        view.canvas()->setImage(&currentImage);
    });

    connect(&view, &MainWindow::moveFrame, this, [this](int firstFrame, int secondFrame) {
        // Save the current frame
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        // Swap frames and set the new current frame index
        model.getFrames().swap(firstFrame, secondFrame);
        model.getCanvasSettings().setCurrentFrameIndex(secondFrame);

        // Set the current image and update canvas
        currentImage = model.getFrames().get(secondFrame);
        view.canvas()->setImage(&currentImage);
    });

    connect(&view, &MainWindow::resizeCanvas, this, [this](int width, int height) {
        // Save the current frame
        model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex()) = currentImage;

        // Resize all existing frames individually
        for (int i = 0; i < model.getFrames().numFrames(); ++i) {
            QImage& frame = model.getFrames().get(i);
            QImage resizedImage(width, height, QImage::Format_RGB32);
            resizedImage.fill(QColor(Qt::white));

            // Copy the desired portion from the original frame to the resized frame
            QPainter painter(&resizedImage);
            painter.drawImage(QPoint(0, 0), frame.copy(0, 0, width, height));
            painter.end();

            // Update the frame in the model
            frame = resizedImage;
        }

        // Set the current image and update canvas
        currentImage = model.getFrames().get(model.getCanvasSettings().getCurrentFrameIndex());
        view.canvas()->setImage(&currentImage);
    });
}

/**
 * @brief Controller::setupAnimationConnections - Sets up connections related to animation connections
 */
void Controller::setupAnimationConnections()
{
    connect(&view, &MainWindow::setFPS, &model, &Model::updateFPS);
    connect(&view, &MainWindow::startAnimation, &model, &Model::updatePlay);

    connect(&view, &MainWindow::toggleAnimation, this, [this]() {
        model.updatePlay(!model.getPlayStatus());
    });

    connect(&model, &Model::updateAnimationPreview, &view, &MainWindow::receiveAnimationFrameData);
}
