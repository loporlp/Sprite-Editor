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
        /// initializes with 1 blank frame of dimension `width` x `height`
        Frames(uint width, uint height);

        /// default initializer; contains only 1 blank 64x64 frame
        Frames();

        /// returns the number of frames in our current animation
        uint numFrames();

        /// returns a reference to the frame at index
        QImage &getFrame(uint index);

        /// returns a reference to the first frame of the animation
        QImage &first();

        /// returns a reference to the last frame of the animation
        QImage &last();

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
        QVector2D canvasSize;

        /// Zoom factor of the canvas. Stored in model so it can be
        /// serialized into our project file.
        float canvasZoom;

    public:
        CanvasSettings(QVector2D canvasSize,
                       QVector2D canvasPosition = QVector2D(0.0, 0.0),
                       float canvasZoom = 1.0);

        /// returns a const reference to our canvas offset position.
        const QVector2D &getCanvasPosition();

        /// updates the `canvasPosition` to the provided `newPosition`
        void setCanvasPosition(QVector2D newPosition);

        /// returns the zoom factor of our canvas
        float getCanvasZoom();

        /// updates the `canvasZoom` to the provided `newZoom`
        void setCanvasZoom(float newZoom);

        /// returns a reference to the frame we are currently displaying
        QImage &getCurrentFrame();

        /// updates which frame we are currently displaying
        void setCurrentFrame(QImage &frameToDisplay);

        /// converts the screen space pixel coordinates provided to the
        /// coordinate space of the sprite the canvas is displaying.
        QVector2D screenSpaceToImageSpace(QVector2D &screenSpace);
    };

private:
    Q_OBJECT

    Frames frames;
    CanvasSettings canvasSettings;

public:
    explicit Model(QObject *parent = nullptr);

    /// returns a reference to our container of frames
    Frames &getFrames();

    /// returns a reference to our `CanvasSettings` class
    CanvasSettings &getCanvasSettings();

signals:
};

#endif // MODEL_H
