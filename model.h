#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <QObject>
#include <QVector2D>

class Model : public QObject
{
public:
    class Frames
    {
        std::vector<QImage> frames;

    public:
        /// returns the number of frames in our current animation
        uint numFrames();

        /// returns a reference to the frame at index
        QImage &getFrame(uint index);

        /// adds an image as the last frame of our animation.
        void pushFrame(QImage frame);

        /// adds an image to our frame at `position`
        void insertFrame(QImage frame, uint index);

        /// deletes the frame at index `position`
        void deleteFrame(uint index);

        /// deletes the last frame of the animation
        void popFrame();
    };

    class CanvasSettings
    {
        /// Offset position of the canvas, to be modified by panning.
        /// Stored in model so it can be serialized into our project file.
        QVector2D canvasPosition;

        /// Zoom factor of the canvas. Stored in model so it can be
        /// serialized into our project file.
        float canvasZoom;

    public:
        /// returns a const reference to our canvas offset position.
        const QVector2D &getCanvasPosition();

        /// updates the `canvasPosition` to the provided `newPosition`
        void setCanvasPosition(QVector2D newPosition);

        /// returns the zoom factor of our canvas
        float getCanvasZoom();

        /// updates the `canvasZoom` to the provided `newZoom`
        void setCanvasZoom(float newZoom);
    };

private:
    Frames frames;
    CanvasSettings canvasSettings;

    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    Frames &getFrames();
    CanvasSettings &getCanvasSettings();

signals:
};

#endif // MODEL_H
