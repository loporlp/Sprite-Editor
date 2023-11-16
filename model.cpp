#include "model.h"
#include <QImage>
#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <algorithm>

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
    , justUndid(false)
{
    connect(&toolBar, &PToolBar::ColorChanged, this, &Model::recievePenColor);
}

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

void Model::Frames::swap(int firstIndex, int secondIndex)
{
    std::iter_swap(frames.begin() + firstIndex, frames.begin() + secondIndex);
}

void Model::Frames::clearFrames()
{
    frames.clear();
}

void Model::Frames::setFramePixel(QImage &frame, int x, int y, uint color)
{
    frame.setPixel(x, y, color);
}

void Model::updateFrame(QImage *image)
{
    qDebug() << "updated frame " << getCanvasSettings().getCurrentFrameIndex();
    QImage imageToUpdate = image->copy();
    frames.get(getCanvasSettings().getCurrentFrameIndex()) = imageToUpdate;
}

void Model::addUndoStack(QImage *image)
{
    QImage imageToAdd = image->copy();
    undoBuffer.push_back(imageToAdd);

    if (justUndid) {
        redoBuffer.clear();
    }
}

void Model::undo()
{
    if (undoBuffer.empty()) {
        return;
    }

    justUndid = true;

    redoBuffer.push_back(frames.get(getCanvasSettings().getCurrentFrameIndex()));

    frames.insert(undoBuffer.back(), getCanvasSettings().getCurrentFrameIndex());

    emit updateCanvas(undoBuffer.back());

    undoBuffer.pop_back();
}

void Model::redo()
{
    if (redoBuffer.empty()) {
        return;
    }

    justUndid = false;

    undoBuffer.push_back(frames.get(getCanvasSettings().getCurrentFrameIndex()));

    frames.insert(redoBuffer.back(), getCanvasSettings().getCurrentFrameIndex());

    emit updateCanvas(redoBuffer.back());

    redoBuffer.pop_back();
}

void Model::clearBuffers()
{
    undoBuffer.clear();
    redoBuffer.clear();
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

void Model::CanvasData::clearCanvasData()
{
    indexOfCurrentFrame = 0;
}

QVector2D Model::CanvasData::screenSpaceToImageSpace(QVector2D &screenSpace)
{
    qWarning("screenSpaceToImageSpace() not yet implemented");
    return QVector2D(0.0, 0.0);
}

void Model::recieveDrawOnEvent(QImage &image, QPoint pos)
{
    int brushSize = toolBar.CurrentTool()->brushSize;

    if (brushSize == 0) {
        toolBar.DrawWithCurrentTool(image, pos);
    } else {
        // Calculate and iterate of neighboring points based on the brush size
        for (int dx = -brushSize; dx <= brushSize; ++dx) {
            for (int dy = -brushSize; dy <= brushSize; ++dy) {
                QPoint currentPos = pos + QPoint(dx, dy);

                // Check if the current position is within the image boundaries
                if (image.rect().contains(currentPos)) {
                    toolBar.DrawWithCurrentTool(image, currentPos);
                }
            }
        }
    }
}

void Model::recievePenColor(QColor color)
{
    toolBar.SetCurrentBrushSettings(toolBar.CurrentTool()->brushSize, color);
    emit sendColor(color);
}

void Model::recieveActiveTool(Tool tool)
{
    if (tool == Tool::Pen) {
        toolBar.UpdateCurrentTool(Tool::Pen);
    } else if (tool == Tool::Eraser) {
        toolBar.UpdateCurrentTool(Tool::Eraser);
    } else if (tool == Tool::Eyedrop) {
        toolBar.UpdateCurrentTool(Tool::Eyedrop);
    } else if (tool == Tool::Bucket) {
        toolBar.UpdateCurrentTool(Tool::Bucket);
    }
}

void Model::recieveBrushSettings(int size, QColor color)
{
    //    toolBar.SetCurrentBrushSettings(size, color);

    toolBar.SetCurrentBrushSettings(size, toolBar.CurrentTool()->brushColor);
}
void Model::updateFPS(int otherFps)
{
    fps = otherFps;
}

void Model::updatePlay(bool otherPlay)
{
    play = otherPlay;
    if (play)
        beginAnimation();
    else
        endAnimation();
}

bool Model::getPlayStatus()
{
    return play;
}


void Model::playAnimationFrames()
{
    if(play)
    {
        // Get how often to change the frame in milliseconds
        int frameTime = calculateDelay();
        int delay = 0;

        qDebug() << (int)frames.numFrames();

        for (int i = 0; i < (int)frames.numFrames(); i++)
        {
            emit updateAnimationPreview(frames.get(i), delay);
            delay += frameTime;
        }

    }
}

void Model::beginAnimation()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Model::playAnimationFrames);

    timer->start(calculateDelay() * frames.numFrames());

}

void Model::endAnimation()
{
    timer->stop();
}

double Model::calculateDelay()
{
    double delay = (1 / ((double)fps)) * 1000;

    return delay;
}

