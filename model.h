#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <QObject>

class Model : public QObject
{
    std::vector<QImage> frames;
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    /// returns the number of frames in our current animation
    uint numFrames();

    /// adds an image as the last frame of our animation.
    void pushFrame(QImage frame);

    /// adds an image to our frame at `position`
    void insertFrame(QImage frame, uint position);

    /// deletes the frame at index `position`
    void deleteFrame(uint position);

    /// deletes the last frame of the animation
    void popFrame();
signals:
};

#endif // MODEL_H
