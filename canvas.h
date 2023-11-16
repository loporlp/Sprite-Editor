#ifndef CANVAS_H
#define CANVAS_H

#include <QGestureEvent>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Canvas;
}
QT_END_NAMESPACE

class Canvas : public QWidget
{
    Q_OBJECT
    QLabel *imageHolder;
    QPoint canvasSize;
    QPoint offset;
    float scaleFactor;

public:
    explicit Canvas(QWidget *parent = nullptr);
    void setImage(QImage *image);
    void setScale(float);
    void setOffset(QPoint);
    void update();
    QImage *imageToDisplay;
    void keyPressEvent(QKeyEvent *event);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    bool event(QEvent *event);

private:
    QPoint canvasToSpriteSpace(QPoint canvasSpace);
    void gestureEvent(QGestureEvent *event);
    void pinchEvent(QPinchGesture *event);

signals:
    void canvasMousePressed(QPoint spriteMouseLocation);
    void canvasMouseMoved(QPoint spriteMouseLocation);
    void canvasMouseReleased(QPoint spriteMouseLocation);
};

#endif // CANVAS_H
