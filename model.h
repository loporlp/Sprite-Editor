#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <QObject>
#include <QVector2D>
#include <QLabel>
#include <QTimer>
#include "PToolBar.h"
#include "PEnums.h"


class Model : public QObject
{


public:

    class Frames
    {
        std::vector<QImage> frames;

    public:
        /// initializes with 1 blank white frame of dimension `width` x `height`
        Frames(uint width, uint height);

        /// default initializer; contains only 1 blank white 64x64 frame
        Frames();

        /// returns the number of frames in our current animation
        uint numFrames();

        /// returns the frame at index
        QImage& get(uint index);

        /// returns the first frame of the animation
        QImage first();

        /// returns the last frame of the animation
        QImage last();

        /// Generates a blank white frame of size width x height
        void generateFrame(int width, int height);

        /// temporary drawing function type beat
        void setFramePixel(QImage &frame, int x, int y, uint color);

        /// adds an image as the last frame of our animation.
        void push(QImage frame);

        /// adds an image to our frame at `position`
        void insert(QImage frame, uint index);

        /// deletes the frame at index `position`
        void remove(uint index);

        /// deletes the last frame of the animation
        void pop();

        /// swap the vector item at the first parameter index with the item at the second parameter index.
        void swap(int firstIndex, int secondIndex);

        /// clear all the data within the frame class object's frame vector
        void clearFrames();
    };

    class CanvasData
    {
        /// Offset position of the canvas, to be modified by panning.
        /// Stored in model so it can be serialized into our project file.
        QVector2D canvasPosition;
        QVector2D canvasSize;

        /// Zoom factor of the canvas. Stored in model so it can be
        /// serialized into our project file.
        float canvasZoom;

        /// The index of the frame that we are currently displaying in the canvas.
        /// An integer rather than a reference for easier serialization.
        uint indexOfCurrentFrame;

    public:
        CanvasData(QVector2D canvasSize,
                   QVector2D canvasPosition = QVector2D(0.0, 0.0),
                   float canvasZoom = 1.0);

        /// returns a const reference to our canvas offset position.
        const QVector2D &getPosition();

        /// updates the `canvasPosition` to the provided `newPosition`
        void setPosition(QVector2D newPosition);

        /// returns the zoom factor of our canvas
        float getZoom();

        /// updates the `canvasZoom` to the provided `newZoom`
        void setZoom(float newZoom);

        /// returns the index of the frame we are currently displaying
        uint getCurrentFrameIndex();

        /// updates the index of which frame we are currently displaying
        void setCurrentFrameIndex(uint newIndex);

        /// resets all the canvas data
        void clearCanvasData();

        /// converts the screen space pixel coordinates provided to the
        /// coordinate space of the sprite the canvas is displaying.
        QVector2D screenSpaceToImageSpace(QVector2D &screenSpace);
    };

private:
    Q_OBJECT

    Frames frames;
    CanvasData canvasSettings;
    PToolBar toolBar;
    bool justUndid;
    int fps = 2;
    bool play = false;
    QTimer *timer;

public:
    explicit Model(QObject *parent = nullptr);
    std::vector<QImage> undoBuffer;
    std::vector<QImage> redoBuffer;

    //adds to the undo stack
    void addUndoStack(QImage *image);

    void undo();

    void redo();

    void updateFrame(QImage *image);

    /// returns a reference to our container of frames
    Frames &getFrames();

    /// returns a reference to our `CanvasSettings` class
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
    void recieveActiveTool(Tool tool);
    void recieveBrushSettings(int size, QColor color);
    void updateFPS(int fps);
    void updatePlay(bool play);

signals:
    void sendColor(QColor color);

    void updateCanvas(QImage& image);

    void updateAnimationPreview(QImage frame, int frameTime);

};

#endif // MODEL_H
