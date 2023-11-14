#include "canvas.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPinchGesture>
#include <QStyle>
#include <QVBoxLayout>

/// Qt lags quite a bit when trying to render an image scaled too large.
const float MAX_ZOOM = 100;
/// How many pixels to move the canvas when the user presses one of the arrow keys
const int KEYBOARD_MOVE_PIXEL_STEP = 1;
/// How many pixels to move the canvas when the user presses one of the arrow keys
/// with the shift key held down.
const int KEYBOARD_MOVE_PIXEL_STEP_SHIFT = 8;

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
    , imageHolder{new QLabel(this)}
    , canvasSize(QPoint(0, 0))
    , offset(QPoint(0, 0))
    , scaleFactor(5)
{

    grabGesture(Qt::PinchGesture);
}

/// Updates which image is being displayed on the canvas. Immediately refreshes by
/// calling `update()`
void Canvas::setImage(QImage *image)
{
    imageToDisplay = image;
    canvasSize = QPoint(image->width(), image->height());
    update();
}

/// Updates the scale, or zoom factor, of the canvas. A scale 8 means a 1x1
/// sprite pixel will take up 8x8 pixels onscreen.
void Canvas::setScale(float newScale)
{
    scaleFactor = newScale;
}

/// Updates the offset position of the canvas for panning. (0, 0) will put the top-left
/// corner of the displayed image in the top-left corner of the frame.
void Canvas::setOffset(QPoint newOffset)
{
    offset = newOffset;
}

/// refresh our rendering of the frame. If any changes have been made to offset,
/// scaleFactor, or the image being displayed, this method needs to be called before
/// you'll see anything appear on screen.
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

/// Maps a coordinate in canvas space (meaning the top left position of the canvas
/// viewport frame is (0, 0) and the bottom right is (canvas.width, canvas.height))
/// to the respective coordinates of the sprite being rendered in the canvas.
///
/// So for example, if the mouse is over a certain red pixel in the sprite, this method
/// will return the QPoint where y is the row and x is the column where that red pixel
/// is stored in the original sprite grid, if given the mouse position in the canvas.
QPoint Canvas::canvasToSpriteSpace(QPoint canvasSpace)
{
    QPoint relative = canvasSpace - offset;
    float x = float(relative.x()) / float(scaleFactor);
    float y = float(relative.y()) / float(scaleFactor);

    return QPoint(floor(x), floor(y));
}

/// Called when a mousePressEvent is detected and emits the `canvasMousePressed()`
/// that maps the mouse position to sprite coordinates, for use by the controller.
void Canvas::mousePressEvent(QMouseEvent *event)
{
    // should probably introduce some bounds checking here
    emit canvasMousePressed(canvasToSpriteSpace(event->pos()));
}

/// Called when a mouseMoveEvent is detected and emits the `canvasMouseMoved()`
/// that maps the mouse position to sprite coordinates, for use by the controller.
void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    emit canvasMouseMoved(canvasToSpriteSpace(event->pos()));
}

/// Called when a mouseReleaseEvent is detected and emits the `canvasMouseReleased()`
/// that maps the mouse position to sprite coordinates, for use by the controller.
void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    emit canvasMouseReleased(canvasToSpriteSpace(event->pos()));
}

/// Called when the user inputs to a horizontal or vertical scroll action to pan the
/// canvas around. Touchpad scrolling is included in that.
void Canvas::wheelEvent(QWheelEvent *event)
{
    this->setOffset(offset + event->pixelDelta());
    this->update();
}

/// Captures and processes gesture events, which are touch inputs. Really just serves
/// to route gestures of different types to their proper method.
///
/// **Note:** `Canvas::event()` will only recognize a gesture if you use
/// `grabGesture(GestureType)` in the canvas constructor, as per Qt's funkiness.
void Canvas::gestureEvent(QGestureEvent *event)
{
    if (QGesture *eventGesture = event->gesture(Qt::PinchGesture)) {
        auto pinch = static_cast<QPinchGesture *>(eventGesture);
        pinchEvent(pinch);
    }
}

/// Processes pinch gestues for zooming. Updates the canvas's scaleFactor and offset
/// so that the pixel underneath the mouse when the user begins zooming stays fixed
/// in place.
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

/// Processes key-down inputs to allow the user to move the canvas around with
/// arrow keys. When shift is held down, the number of pixels moved per input changes
/// to `KEYBOARD_MOVE_PIXEL_STEP_SHIFT`
///
/// Since only one widget can listen to keyboard inputs at a time, and the MainWindow
/// needs to listen for Undo/Redo actions, the event is caught in `MainWindow::keyPressEvent`
/// and piped here.
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

/// in order to use Gestures (pinch zoom and all that) we have to override the default
/// event method :/
///
/// Detects what type of event the user is inputting, and calls the appropriate handler
/// method for inputs of that type, passing in the event after appropriately casted.
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

    default: {
        // event of a different typ
    }
    }
    return true;
}
