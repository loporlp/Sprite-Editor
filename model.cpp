#include "model.h"
#include <QObject>

Model::Model(QObject *parent)
    : QObject{parent}
    , frames{Model::Frames()}
    , canvasSettings{Model::CanvasSettings()}
{}

//-----Model::Frames-----//
uint Model::Frames::numFrames()
{
    return frames.size();
}

QImage &Model::Frames::getFrame(uint index)
{
    assert(frames.size() > index);
    return frames.at(index);
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
