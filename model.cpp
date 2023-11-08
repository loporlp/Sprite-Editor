#include "model.h"
#include <QImage>
#include <QObject>
#include <QPixmap>

//-----Model-----//
Model::Frames &Model::getFrames()
{
    return frames;
}

Model::CanvasSettings &Model::getCanvasSettings()
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
    QImage defaultFrame = QPixmap(width, height).toImage();
    frames.push_back(defaultFrame);
}

Model::Frames::Frames()
    : Model::Frames::Frames(64, 64)
{}

uint Model::Frames::numFrames()
{
    return frames.size();
}

QImage &Model::Frames::getFrame(uint index)
{
    assert(frames.size() > index);
    return frames.at(index);
}

QImage &Model::Frames::first()
{
    return frames.front();
}

QImage &Model::Frames::last()
{
    return frames.back();
}

void Model::Frames::pushFrame(QImage frame)
{
    frames.push_back(frame);
}

void Model::Frames::insertFrame(QImage frame, uint index)
{
    frames.insert(frames.begin() + index, frame);
}

void Model::Frames::deleteFrame(uint index)
{
    assert(frames.size() > index);
    frames.erase(frames.begin() + index);
}

void Model::Frames::popFrame()
{
    frames.pop_back();
}

//-----Model::CanvasSettings-----//

Model::CanvasSettings::CanvasSettings(QVector2D canvasSize,
                                      QVector2D canvasPosition,
                                      float canvasZoom)
    : canvasPosition(canvasPosition)
    , canvasSize(canvasSize)
    , canvasZoom(canvasZoom)
{}

const QVector2D &Model::CanvasSettings::getCanvasPosition()
{
    return canvasPosition;
}

void Model::CanvasSettings::setCanvasPosition(QVector2D newPosition)
{
    canvasPosition = newPosition;
}

float Model::CanvasSettings::getCanvasZoom()
{
    return canvasZoom;
}

void Model::CanvasSettings::setCanvasZoom(float newZoom)
{
    canvasZoom = newZoom;
}

uint Model::CanvasSettings::getCurrentFrameIndex()
{
    return indexOfCurrentFrame;
}

void Model::CanvasSettings::setCurrentFrameIndex(uint newIndex)
{
    indexOfCurrentFrame = newIndex;
}

QVector2D Model::CanvasSettings::screenSpaceToImageSpace(QVector2D &screenSpace)
{
    qWarning("screenSpaceToImageSpace() not yet implemented");
    return QVector2D(0.0, 0.0);
}
