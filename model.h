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
signals:
};

#endif // MODEL_H
