#include "model.h"
#include <QImage>
#include <QLabel>
#include <QObject>
#include <QPixmap>

#include <QDebug>

//-----Model-----//
Model::Frames &Model::getFrames()
{
    return frames;
}

Model::CanvasData &Model::getCanvasSettings()
{
    return canvasSettings;
}

Model::Model(QObject *parent)
    : QObject(parent)
    , frames()
    , canvasSettings(QVector2D(frames.first().width(), frames.first().height()))
{}

//-----Model::Frames-----//

Model::Frames::Frames(uint width, uint height)
{
    generateFrame(width, height);
}

Model::Frames::Frames()
    : Model::Frames::Frames(64, 64)
{}

void Model::Frames::generateFrame(int width, int height)
{
    QImage defaultFrame(width, height, QImage::Format_RGB32);
    defaultFrame.fill(QColor(Qt::white));
    frames.push_back(defaultFrame);
}

uint Model::Frames::numFrames()
{
    return frames.size();
}

QImage &Model::Frames::get(uint index)
{
    assert(frames.size() > index);
    return frames.at(index);
}

QImage Model::Frames::first()
{
    return frames.front();
}

QImage Model::Frames::last()
{
    return frames.back();
}

void Model::Frames::push(QImage frame)
{
    frames.push_back(frame);
}

void Model::Frames::insert(QImage frame, uint index)
{
    frames.insert(frames.begin() + index, frame);
}

void Model::Frames::remove(uint index)
{
    assert(frames.size() > index);
    frames.erase(frames.begin() + index);
}

void Model::Frames::pop()
{
    frames.pop_back();
}

void Model::addUndoStack(QImage* image)
{
    QImage imageToAdd = image->copy();
    qDebug() << "ADDED TO BUFFER";
    undoBuffer.push_back(imageToAdd);
    qDebug() << &imageToAdd;
}

void Model::undo()
{
    if (undoBuffer.empty()) {
        return;
    }

    qDebug() << "UNDO";
    //QImage imageToUndo = undoBuffer.back();
    qDebug() << undoBuffer.back();
    // frames.insert(undoBuffer.back(), 0);
    emit updateCanvas(undoBuffer.back());
    //delete undoBuffer.back();
    undoBuffer.pop_back();
}

//-----Model::CanvasData-----//

Model::CanvasData::CanvasData(QVector2D canvasSize, QVector2D canvasPosition, float canvasZoom)
    : canvasPosition(canvasPosition)
    , canvasSize(canvasSize)
    , canvasZoom(canvasZoom)
    , indexOfCurrentFrame(0)
{}

const QVector2D &Model::CanvasData::getPosition()
{
    return canvasPosition;
}

void Model::CanvasData::setPosition(QVector2D newPosition)
{
    canvasPosition = newPosition;
}

float Model::CanvasData::getZoom()
{
    return canvasZoom;
}

void Model::CanvasData::setZoom(float newZoom)
{
    canvasZoom = newZoom;
}

uint Model::CanvasData::getCurrentFrameIndex()
{
    return indexOfCurrentFrame;
}

void Model::CanvasData::setCurrentFrameIndex(uint newIndex)
{
    indexOfCurrentFrame = newIndex;
}

QVector2D Model::CanvasData::screenSpaceToImageSpace(QVector2D &screenSpace)
{
    qWarning("screenSpaceToImageSpace() not yet implemented");
    return QVector2D(0.0, 0.0);
}
