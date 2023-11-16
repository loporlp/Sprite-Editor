/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): David Cosby, Andrew Wilhelm, Allison Walker,
 * Mason Sansom, AJ Kennedy, Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * Model Source
 *
 * File reviewed by: Brett Baxter, Allison Walker
 *
 * Brief:
 * The Model class represents the core
 * Frame and Canvas relation logic of
 * the Pixel Image Software Suite (PISS).
 *
 *
*/

#include <QImage>
#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <algorithm>

#include "model.h"

//-----Model-----//

/**
 * @brief Model::getFrames - Returns a reference to the frames container
 * @return
 */
Model::Frames &Model::getFrames()
{
    return frames;
}

/**
 * @brief Model::getCanvasSettings - Returns a reference to the canvas settings
 * @return
 */
Model::CanvasData &Model::getCanvasSettings()
{
    return canvasSettings;
}

/**
 * @brief Model::Model - Constructor for the Model class
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
 * @brief Model::Frames::Frames - Constructor for the Frames class
 * @param width
 * @param height
 */
Model::Frames::Frames(uint width, uint height)
{
    generateFrame(width, height);
}

/**
 * @brief Model::Frames::Frames - Default constructor for Frames with default size
 */
Model::Frames::Frames()
    : Model::Frames::Frames(64, 64)
{}

/**
 * @brief Model::Frames::generateFrame - Generate a new frame with the specified width and height
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
 * @brief Model::Frames::numFrames - Returns the number of frames in our vector
 * @return
 */
uint Model::Frames::numFrames()
{
    return frames.size();
}

/**
 * @brief Model::Frames::get - Gets the frame at a specifc indoex in our vector
 * @param index
 * @return
 */
QImage &Model::Frames::get(uint index)
{
    assert(frames.size() > index);
    return frames.at(index);
}

/**
 * @brief Model::Frames::first - Returns the frame at the front of our vector
 * @return
 */
QImage Model::Frames::first()
{
    return frames.front();
}

/**
 * @brief Model::Frames::last - Returns the frame at the back of our vector
 * @return
 */
QImage Model::Frames::last()
{
    return frames.back();
}

/**
 * @brief Model::Frames::push - Pushes back the input parameter frame into our frame vector
 * @param frame
 */
void Model::Frames::push(QImage frame)
{
    frames.push_back(frame);
}

/**
 * @brief Model::Frames::insert - Inserts the parameter frame at the specified index
 * @param frame
 * @param index
 */
void Model::Frames::insert(QImage frame, uint index)
{
    frames.insert(frames.begin() + index, frame);
}

/**
 * @brief Model::Frames::remove - Removes the frame at the specifc index in our frame vector
 * @param index
 */
void Model::Frames::remove(uint index)
{
    assert(frames.size() > index);
    frames.erase(frames.begin() + index);
}

/**
 * @brief Model::Frames::pop - Pops the frame in our vector
 */
void Model::Frames::pop()
{
    frames.pop_back();
}

/**
 * @brief Model::Frames::swap - Swaps two frames in our vector at the specifed index parameters
 * @param firstIndex
 * @param secondIndex
 */
void Model::Frames::swap(int firstIndex, int secondIndex)
{
    std::iter_swap(frames.begin() + firstIndex, frames.begin() + secondIndex);
}

/**
 * @brief Model::Frames::clearFrames - Clears our entire vector of frames
 */
void Model::Frames::clearFrames()
{
    frames.clear();
}

/**
 * @brief Model::Frames::setFramePixel - Sets a pixel point of the specific frame with a specific color
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
 * @brief Model::updateFrame - Update the current frame with a new image
 * @param image
 */
void Model::updateFrame(QImage *image)
{
    QImage imageToUpdate = image->copy();
    frames.get(getCanvasSettings().getCurrentFrameIndex()) = imageToUpdate;
}

/**
 * @brief Model::addUndoStack - Adds frame image to the undo stack
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
 * @brief Model::undo - Undo logic
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
 * @brief Model::redo - Redo logic
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
 * @brief Model::clearBuffers - Clears our undo and redo buffers
 */
void Model::clearBuffers()
{
    undoBuffer.clear();
    redoBuffer.clear();
}

//-----Model::CanvasData-----//

/**
 * @brief Model::CanvasData::CanvasData - Constructor for the CanvasData class
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
 * @brief Model::CanvasData::getPosition - Canvas position getter
 * @return
 */
const QVector2D &Model::CanvasData::getPosition()
{
    return canvasPosition;
}

/**
 * @brief Model::CanvasData::setPosition - Canvas position setter
 * @param newPosition
 */
void Model::CanvasData::setPosition(QVector2D newPosition)
{
    canvasPosition = newPosition;
}

/**
 * @brief Model::CanvasData::getZoom - Canvas zoom scale getter
 * @return
 */
float Model::CanvasData::getZoom()
{
    return canvasZoom;
}

/**
 * @brief Model::CanvasData::setZoom - Canvas zoom scale setter
 * @param newZoom
 */
void Model::CanvasData::setZoom(float newZoom)
{
    canvasZoom = newZoom;
}

/**
 * @brief Model::CanvasData::getCurrentFrameIndex - Gets the index of our current frame
 * @return
 */
uint Model::CanvasData::getCurrentFrameIndex()
{
    return indexOfCurrentFrame;
}

/**
 * @brief Model::CanvasData::setCurrentFrameIndex - - Gets the index of our current frame
 * @param newIndex
 */
void Model::CanvasData::setCurrentFrameIndex(uint newIndex)
{
    indexOfCurrentFrame = newIndex;
}

/**
 * @brief Model::CanvasData::clearCanvasData - Clears index and data for our canvas
 */
void Model::CanvasData::clearCanvasData()
{
    indexOfCurrentFrame = 0;
}

/**
 * @brief Model::recieveDrawOnEvent - Receives a draw event and process it
 * @param image
 * @param pos
 */
void Model::recieveDrawOnEvent(QImage &image, QPoint pos)
{
    int brushSize = toolBar.CurrentTool()->brushSize;

    if (brushSize == 0) {
        toolBar.drawWithCurrentTool(image, pos);
    } else {

        // Calculate and iterate the neighboring points based on the brush size
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
 * @brief Model::recievePenColor - Receive a pen color and process it
 * @param color
 */
void Model::recievePenColor(QColor color)
{
    toolBar.setCurrentBrushSettings(toolBar.CurrentTool()->brushSize, color);
    emit sendColor(color);
}

/**
 * @brief Model::recieveActiveTool - Receive the active tool and process it
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
 * @brief Model::recieveBrushSettings - Receive the current brush settings and process it
 * @param size
 * @param color
 */
void Model::recieveBrushSettings(int size, QColor color)
{
    toolBar.setCurrentBrushSettings(size, toolBar.CurrentTool()->brushColor);
}

/**
 * @brief Model::updateFPS - Update the frames per second (FPS) value
 * @param otherFps
 */
void Model::updateFPS(int otherFps)
{
    fps = otherFps;
}

/**
 * @brief Model::updatePlay - Update the play status and handle animation accordingly
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
 * @brief Model::getPlayStatus - Returns status of if the animation is playing
 * @return
 */
bool Model::getPlayStatus()
{
    return play;
}

/**
 * @brief Model::playAnimationFrames - Controls the animation of the current frames
 */
void Model::playAnimationFrames()
{
    if(play)
    {
        // Get how often to change the frame in milliseconds
        int frameTime = calculateDelay();
        int delay = 0;

        for (int i = 0; i < (int)frames.numFrames(); i++)
        {
            emit updateAnimationPreview(frames.get(i), delay);
            delay += frameTime;
        }

    }
}

/**
 * @brief Model::beginAnimation - Starts the animation of the current frames
 */
void Model::beginAnimation()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Model::playAnimationFrames);

    timer->start(calculateDelay() * frames.numFrames());

}

/**
 * @brief Model::endAnimation - Ends the animation of the current frames
 */
void Model::endAnimation()
{
    timer->stop();
}

/**
 * @brief Model::calculateDelay - Calculate the delay between animation frames based on the FPS
 * @return
 */
double Model::calculateDelay()
{
    double delay = (1 / ((double)fps)) * 1000;

    return delay;
}

