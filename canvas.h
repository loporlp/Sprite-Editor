/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): David Cosby, Andrew Wilhelm, Allison Walker,
 * Mason Sansom, AJ Kennedy, Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * Canvas Header
 *
 * File reviewed by: Brett Baxter, Allison Walker
 *
 * Brief:
 * The Canvas class is responsible
 * for handling image display and user interactions.
 *
 *
*/

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

    /// Items to help display, size and scale the image
    QLabel *imageHolder;
    QPoint canvasSize;
    QPoint offset;
    float scaleFactor;

public:
    explicit Canvas(QWidget *parent = nullptr);

    /// Setters for canvas properties
    void setImage(QImage *image);
    void setScale(float);
    void setOffset(QPoint);

    /// Update the canvas display
    void update();

    /// Image to be displayed on the canvas
    QImage *imageToDisplay;

    /// Handle key press events
    void keyPressEvent(QKeyEvent *event);

protected:
    /// Event handlers for mouse and wheel events
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    /// Generic event handler for events
    bool event(QEvent *event);

private:
    /// Convert canvas space coordinates to sprite space coordinates
    QPoint canvasToSpriteSpace(QPoint canvasSpace);

    /// Event handlers
    void gestureEvent(QGestureEvent *event);
    void pinchEvent(QPinchGesture *event);

    /// Zoom at a specific point on the canvas
    void zoomAtPoint(float scaleFactorMultiplier, QPointF mousePos);

signals:
    /// Signals for notifying about mouse interactions on the canvas
    void canvasMousePressed(QPoint spriteMouseLocation);
    void canvasMouseMoved(QPoint spriteMouseLocation);
    void canvasMouseReleased(QPoint spriteMouseLocation);
};

#endif // CANVAS_H
