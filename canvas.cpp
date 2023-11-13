#include "canvas.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPinchGesture>
#include <QStyle>
#include <QVBoxLayout>

// Qt lags quite a bit when trying to render an image scaled too large.
const float MAX_ZOOM = 100;

/// How many pixels to move the canvas when the user presses one of the arrow keys
const int KEYBOARD_MOVE_PIXEL_STEP = 1;
/// How many pixels to move the canvas when the user presses one of the arrow keys with the shift key held down.
const int KEYBOARD_MOVE_PIXEL_STEP_SHIFT = 8;

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
    , imageHolder{new QLabel(this)}
    , canvasSize(QPoint(0, 0))
    , offset(QPoint(0, 0))
    , scaleFactor(5)
{
    this->setStyleSheet("QWidget {background-color: rgb(200, 255, 255)}");

    grabGesture(Qt::PinchGesture);
    this->grabKeyboard();
}

void Canvas::setImage(QImage *image)
{
    imageToDisplay = image;
    canvasSize = QPoint(image->width(), image->height());
    update();
}

void Canvas::setScale(float newScale)
{
    scaleFactor = newScale;
}

void Canvas::setOffset(QPoint newOffset)
{
    offset = newOffset;
}

void Canvas::update()
{
    QImage scaled = imageToDisplay->scaled(canvasSize.x() * scaleFactor,
                                           canvasSize.y() * scaleFactor);

    imageHolder->setPixmap(QPixmap::fromImage(scaled));
    imageHolder->setGeometry(offset.x(),
                             offset.y(),
                             canvasSize.x() * scaleFactor,
                             canvasSize.y() * scaleFactor);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    // should probably introduce some bounds checking here
    emit canvasMousePressed(canvasToSpriteSpace(event->pos() - offset));
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    emit canvasMouseMoved(canvasToSpriteSpace(event->pos() - offset));
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    emit canvasMouseReleased(canvasToSpriteSpace(event->pos() - offset));
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    this->setOffset(offset + event->pixelDelta());
    this->update();
}

void Canvas::gestureEvent(QGestureEvent *event)
{
    if (QGesture *eventGesture = event->gesture(Qt::PinchGesture)) {
        auto pinch = static_cast<QPinchGesture *>(eventGesture);
        pinchEvent(pinch);
    }
}

void Canvas::pinchEvent(QPinchGesture *event)
{
    scaleFactor = fmin(MAX_ZOOM, scaleFactor * event->scaleFactor());
    if (scaleFactor == MAX_ZOOM) {
        return;
    }

    QPointF centerRelative = offset - event->centerPoint();
    QPointF centerScaled = centerRelative * event->scaleFactor();

    QPointF translation = (centerScaled - centerRelative);
    offset += translation.toPoint();

    update();
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    QPoint moveDirection = QPoint(0, 0);
    if (event->key() == Qt::Key_Left) {
        moveDirection = QPoint(1, 0);
    } else if (event->key() == Qt::Key_Right) {
        moveDirection = QPoint(-1, 0);
    } else if (event->key() == Qt::Key_Up) {
        moveDirection = QPoint(0, 1);
    } else if (event->key() == Qt::Key_Down) {
        moveDirection = QPoint(0, -1);
    }

    if (moveDirection.manhattanLength() != 0) {
        int pixelsToMove = KEYBOARD_MOVE_PIXEL_STEP;
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            pixelsToMove = KEYBOARD_MOVE_PIXEL_STEP_SHIFT;
        }

        offset += (moveDirection * pixelsToMove * scaleFactor);
        update();
    }
}

// in order to use Gestures (pinch zoom and all that) we have to override the default event method :/
bool Canvas::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Gesture: {
        gestureEvent(static_cast<QGestureEvent *>(event));
        break;
    }
    case QEvent::MouseButtonPress: {
        mousePressEvent(static_cast<QMouseEvent *>(event));
        break;
    }

    case QEvent::MouseMove: {
        mouseMoveEvent(static_cast<QMouseEvent *>(event));
        break;
    }

    case QEvent::MouseButtonRelease: {
        mouseReleaseEvent(static_cast<QMouseEvent *>(event));
        break;
    }

    case QEvent::Wheel: {
        wheelEvent(static_cast<QWheelEvent *>(event));
        break;
    }

    case QEvent::KeyPress: {
        keyPressEvent(static_cast<QKeyEvent *>(event));
        break;
    }

    default: {
        // event of a different typ
    }
    }
    return true;
}

QPoint Canvas::canvasToSpriteSpace(QPoint canvasSpace)
{
    float x = float(canvasSpace.x()) / float(scaleFactor);
    float y = float(canvasSpace.y()) / float(scaleFactor);

    return QPoint(floor(x), floor(y));
}
