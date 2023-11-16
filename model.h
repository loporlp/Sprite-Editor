/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): David Cosby, Andrew Wilhelm, Allison Walker,
 * Mason Sansom, AJ Kennedy, Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * Model Header
 *
 * File reviewed by: Brett Baxter, Allison Walker
 *
 * Brief:
 * The Model class represents the core
 * Frame and Canvas relation logic of
 * the Pixel Image Software Suite (PISS).
 *
*/

#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <QLabel>
#include <QObject>
#include <QVector2D>
#include <QLabel>
#include <QTimer>

#include "toolbar.h"
#include "enums.h"
#include "toolbar.h"


class Model : public QObject
{
public:

    /// Class for managing frames in the animation
    class Frames
    {
        std::vector<QImage> frames;

    public:
        /// Initializes with 1 blank white frame of dimension `width` x `height`
        Frames(uint width, uint height);

        /// Default initializer; contains only 1 blank white 64x64 frame
        Frames();

        /// Returns the number of frames in our current animation
        uint numFrames();

        /// Returns the frame at index
        QImage &get(uint index);

        /// returns the first and last frame of the animation
        QImage first();
        QImage last();

        /// Generates a blank white frame of size width x height
        void generateFrame(int width, int height);

        /// Sets a frame pixel with the drawn color
        void setFramePixel(QImage &frame, int x, int y, uint color);

        /// Adds an image as the last frame of our animation.
        void push(QImage frame);

        /// Adds an image to our frame at `position`
        void insert(QImage frame, uint index);

        /// Deletes the frame at index `position`
        void remove(uint index);

        /// Deletes the last frame of the animation
        void pop();

        /// Swap the vector item at the first parameter index with the item at the second parameter index
        void swap(int firstIndex, int secondIndex);

        /// Clear all the data within the frame class object's frame vector
        void clearFrames();
    };

    /// Class for managing canvas data
    class CanvasData
    {
        /// Offset position of the canvas, to be modified by panning
        /// Stored in model so it can be serialized into our project file
        QVector2D canvasPosition;
        QVector2D canvasSize;

        /// Zoom factor of the canvas. Stored in model so it can be
        /// serialized into our project file
        float canvasZoom;

        /// The index of the frame that we are currently displaying in the canvas
        /// An integer rather than a reference for easier serialization
        uint indexOfCurrentFrame;

    public:
        CanvasData(QVector2D canvasSize,
                   QVector2D canvasPosition = QVector2D(0.0, 0.0),
                   float canvasZoom = 1.0);

        /// Returns a const reference to our canvas offset position
        const QVector2D &getPosition();

        /// Updates the `canvasPosition` to the provided `newPosition`
        void setPosition(QVector2D newPosition);

        /// Returns the zoom factor of our canvas
        float getZoom();

        /// updates the `canvasZoom` to the provided `newZoom`
        void setZoom(float newZoom);

        /// Returns the index of the frame we are currently displaying
        uint getCurrentFrameIndex();

        /// Updates the index of which frame we are currently displaying
        void setCurrentFrameIndex(uint newIndex);

        /// Resets all the canvas data
        void clearCanvasData();
    };

private:
    Q_OBJECT

    Frames frames;
    CanvasData canvasSettings;
    ToolBar toolBar;
    bool justUndid;
    int fps = 2;
    bool play = false;
    QTimer *timer;

public:
    explicit Model(QObject *parent = nullptr);
    std::vector<QImage> undoBuffer;
    std::vector<QImage> redoBuffer;

    /// Adds to the undo stack
    void addUndoStack(QImage *image);
    void clearBuffers();
    void undo();
    void redo();
    void updateFrame(QImage *image);

    /// Returns a reference to our container of frames
    Frames &getFrames();

    /// Returns a reference to our `CanvasSettings` class
    CanvasData &getCanvasSettings();

    /// Animation Methods
    void playAnimationFrames();
    void beginAnimation();
    void endAnimation();
    bool getPlayStatus();
    double calculateDelay();

public slots:
    void recieveDrawOnEvent(QImage &image, QPoint pos);
    void recievePenColor(QColor color);
    void recieveActiveTool(ToolType tool);
    void recieveBrushSettings(int size, QColor color);
    void updateFPS(int fps);
    void updatePlay(bool play);

signals:
    void sendColor(QColor color);
    void updateCanvas(QImage& image);
    void updateAnimationPreview(QImage frame, int frameTime);
};

#endif // MODEL_H
