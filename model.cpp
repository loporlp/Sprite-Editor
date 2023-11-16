/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): -----------
 * Course: CS 3505
 * Fall 2023
 *
 * Model Source
 *
 * Brief:
 *
 *
*/

#include "model.h"
#include <QImage>
#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <algorithm>

#include <QDebug>

//-----Model-----//

/**
 * @brief Model::getFrames
 * @return
 */
Model::Frames &Model::getFrames()
{
    return frames;
}

/**
 * @brief Model::getCanvasSettings
 * @return
 */
Model::CanvasData &Model::getCanvasSettings()
{
    return canvasSettings;
}

/**
 * @brief Model::Model
 * @param parent
 */
Model::Model(QObject *parent)
    : QObject(parent)
    , frames()
    , canvasSettings(QVector2D(frames.first().width(), frames.first().height()))
    , justUndid(false)
{
    connect(&toolBar, &ToolBar::colorChanged, this, &Model::recievePenColor);
}

//-----Model::Frames-----//

/**
 * @brief Model::Frames::Frames
 * @param width
 * @param height
 */
Model::Frames::Frames(uint width, uint height)
{
    generateFrame(width, height);
}

/**
 * @brief Model::Frames::Frames
 */
Model::Frames::Frames()
    : Model::Frames::Frames(64, 64)
{}

/**
 * @brief Model::Frames::generateFrame
 * @param width
 * @param height
 */
void Model::Frames::generateFrame(int width, int height)
{
    QImage defaultFrame(width, height, QImage::Format_RGB32);
    defaultFrame.fill(QColor(Qt::white));
    frames.push_back(defaultFrame);
}

/**
 * @brief Model::Frames::numFrames
 * @return
 */
uint Model::Frames::numFrames()
{
    return frames.size();
}

/**
 * @brief Model::Frames::get
 * @param index
 * @return
 */
QImage &Model::Frames::get(uint index)
{
    assert(frames.size() > index);
    return frames.at(index);
}

/**
 * @brief Model::Frames::first
 * @return
 */
QImage Model::Frames::first()
{
    return frames.front();
}

/**
 * @brief Model::Frames::last
 * @return
 */
QImage Model::Frames::last()
{
    return frames.back();
}

/**
 * @brief Model::Frames::push
 * @param frame
 */
void Model::Frames::push(QImage frame)
{
    frames.push_back(frame);
}

/**
 * @brief Model::Frames::insert
 * @param frame
 * @param index
 */
void Model::Frames::insert(QImage frame, uint index)
{
    frames.insert(frames.begin() + index, frame);
}

/**
 * @brief Model::Frames::remove
 * @param index
 */
void Model::Frames::remove(uint index)
{
    assert(frames.size() > index);
    frames.erase(frames.begin() + index);
}

/**
 * @brief Model::Frames::pop
 */
void Model::Frames::pop()
{
    frames.pop_back();
}

/**
 * @brief Model::Frames::swap
 * @param firstIndex
 * @param secondIndex
 */
void Model::Frames::swap(int firstIndex, int secondIndex)
{
    std::iter_swap(frames.begin() + firstIndex, frames.begin() + secondIndex);
}

/**
 * @brief Model::Frames::clearFrames
 */
void Model::Frames::clearFrames()
{
    frames.clear();
}

/**
 * @brief Model::Frames::setFramePixel
 * @param frame
 * @param x
 * @param y
 * @param color
 */
void Model::Frames::setFramePixel(QImage &frame, int x, int y, uint color)
{
    frame.setPixel(x, y, color);
}

/**
 * @brief Model::updateFrame
 * @param image
 */
void Model::updateFrame(QImage *image)
{
    qDebug() << "updated frame " << getCanvasSettings().getCurrentFrameIndex();
    QImage imageToUpdate = image->copy();
    frames.get(getCanvasSettings().getCurrentFrameIndex()) = imageToUpdate;
}

/**
 * @brief Model::addUndoStack
 * @param image
 */
void Model::addUndoStack(QImage *image)
{
    QImage imageToAdd = image->copy();
    undoBuffer.push_back(imageToAdd);

    if (justUndid)
    {
        redoBuffer.clear();
    }
}

/**
 * @brief Model::undo
 */
void Model::undo()
{
    if (undoBuffer.empty())
    {
        return;
    }

    justUndid = true;

    redoBuffer.push_back(frames.get(getCanvasSettings().getCurrentFrameIndex()));

    frames.get(getCanvasSettings().getCurrentFrameIndex()) = undoBuffer.back();

    emit updateCanvas(undoBuffer.back());

    undoBuffer.pop_back();
}

/**
 * @brief Model::redo
 */
void Model::redo()
{
    if (redoBuffer.empty())
    {
        return;
    }

    justUndid = false;

    undoBuffer.push_back(frames.get(getCanvasSettings().getCurrentFrameIndex()));

    frames.get(getCanvasSettings().getCurrentFrameIndex()) = redoBuffer.back();

    emit updateCanvas(redoBuffer.back());

    redoBuffer.pop_back();
}

/**
 * @brief Model::clearBuffers
 */
void Model::clearBuffers()
{
    undoBuffer.clear();
    redoBuffer.clear();
}

//-----Model::CanvasData-----//

/**
 * @brief Model::CanvasData::CanvasData
 * @param canvasSize
 * @param canvasPosition
 * @param canvasZoom
 */
Model::CanvasData::CanvasData(QVector2D canvasSize, QVector2D canvasPosition, float canvasZoom)
    : canvasPosition(canvasPosition)
    , canvasSize(canvasSize)
    , canvasZoom(canvasZoom)
    , indexOfCurrentFrame(0)
{}

/**
 * @brief Model::CanvasData::getPosition
 * @return
 */
const QVector2D &Model::CanvasData::getPosition()
{
    return canvasPosition;
}

/**
 * @brief Model::CanvasData::setPosition
 * @param newPosition
 */
void Model::CanvasData::setPosition(QVector2D newPosition)
{
    canvasPosition = newPosition;
}

/**
 * @brief Model::CanvasData::getZoom
 * @return
 */
float Model::CanvasData::getZoom()
{
    return canvasZoom;
}

/**
 * @brief Model::CanvasData::setZoom
 * @param newZoom
 */
void Model::CanvasData::setZoom(float newZoom)
{
    canvasZoom = newZoom;
}

/**
 * @brief Model::CanvasData::getCurrentFrameIndex
 * @return
 */
uint Model::CanvasData::getCurrentFrameIndex()
{
    return indexOfCurrentFrame;
}

/**
 * @brief Model::CanvasData::setCurrentFrameIndex
 * @param newIndex
 */
void Model::CanvasData::setCurrentFrameIndex(uint newIndex)
{
    indexOfCurrentFrame = newIndex;
}

/**
 * @brief Model::CanvasData::clearCanvasData
 */
void Model::CanvasData::clearCanvasData()
{
    indexOfCurrentFrame = 0;
}

/**
 * @brief Model::CanvasData::screenSpaceToImageSpace
 * @param screenSpace
 * @return
 */
QVector2D Model::CanvasData::screenSpaceToImageSpace(QVector2D &screenSpace)
{
    qWarning("screenSpaceToImageSpace() not yet implemented");
    return QVector2D(0.0, 0.0);
}

/**
 * @brief Model::recieveDrawOnEvent
 * @param image
 * @param pos
 */
void Model::recieveDrawOnEvent(QImage &image, QPoint pos)
{
    int brushSize = toolBar.CurrentTool()->brushSize;

    if (brushSize == 0) {
        toolBar.drawWithCurrentTool(image, pos);
    } else {
        // Calculate and iterate of neighboring points based on the brush size
        for (int dx = -brushSize; dx <= brushSize; ++dx) {
            for (int dy = -brushSize; dy <= brushSize; ++dy) {
                QPoint currentPos = pos + QPoint(dx, dy);

                // Check if the current position is within the image boundaries
                if (image.rect().contains(currentPos)) {
                    toolBar.drawWithCurrentTool(image, currentPos);
                }
            }
        }
    }
}

/**
 * @brief Model::recievePenColor
 * @param color
 */
void Model::recievePenColor(QColor color)
{
    toolBar.setCurrentBrushSettings(toolBar.CurrentTool()->brushSize, color);
    emit sendColor(color);
}

/**
 * @brief Model::recieveActiveTool
 * @param tool
 */
void Model::recieveActiveTool(ToolType tool)
{
    if (tool == ToolType::Pen)
    {
        toolBar.updateCurrentTool(ToolType::Pen);
    }
    else if (tool == ToolType::Eraser)
    {
        toolBar.updateCurrentTool(ToolType::Eraser);
    }
    else if (tool == ToolType::Eyedrop)
    {
        toolBar.updateCurrentTool(ToolType::Eyedrop);
    }
    else if (tool == ToolType::Bucket)
    {
        toolBar.updateCurrentTool(ToolType::Bucket);
    }
}

/**
 * @brief Model::recieveBrushSettings
 * @param size
 * @param color
 */
void Model::recieveBrushSettings(int size, QColor color)
{
    //    toolBar.SetCurrentBrushSettings(size, color);

    toolBar.setCurrentBrushSettings(size, toolBar.CurrentTool()->brushColor);
}

/**
 * @brief Model::updateFPS
 * @param otherFps
 */
void Model::updateFPS(int otherFps)
{
    fps = otherFps;
}

/**
 * @brief Model::updatePlay
 * @param otherPlay
 */
void Model::updatePlay(bool otherPlay)
{
    play = otherPlay;
    if (play)
    {
        beginAnimation();
    }
    else
    {
        endAnimation();
    }
}

/**
 * @brief Model::getPlayStatus
 * @return
 */
bool Model::getPlayStatus()
{
    return play;
}

/**
 * @brief Model::playAnimationFrames
 */
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

/**
 * @brief Model::beginAnimation
 */
void Model::beginAnimation()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Model::playAnimationFrames);

    timer->start(calculateDelay() * frames.numFrames());

}

/**
 * @brief Model::endAnimation
 */
void Model::endAnimation()
{
    timer->stop();
}

/**
 * @brief Model::calculateDelay
 * @return
 */
double Model::calculateDelay()
{
    double delay = (1 / ((double)fps)) * 1000;

    return delay;
}

