/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): David Cosby, Andrew Wilhelm, Allison Walker,
 * Mason Sansom, AJ Kennedy, Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * Canvas Source
 *
 * File reviewed by: Brett Baxter, Allison Walker
 *
 * Brief:
 * The Canvas class is responsible
 * for handling image display and user interactions.
 *
 *
*/

#include <QLabel>
#include <QMouseEvent>
#include <QPinchGesture>
#include <QStyle>
#include <QVBoxLayout>

#include "canvas.h"

/// Qt lags quite a bit when trying to render an image scaled too large
const float MAX_ZOOM = 100;
/// How many pixels to move the canvas when the user presses one of the arrow keys
const int KEYBOARD_MOVE_PIXEL_STEP = 1;
/// How many pixels to move the canvas when the user presses one of the arrow keys
/// with the shift key held down
const int KEYBOARD_MOVE_PIXEL_STEP_SHIFT = 8;

/**
 * @brief Canvas::Canvas - Constructor
 * @param parent
 */
Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
    , imageHolder{new QLabel(this)}
    , canvasSize(QPoint(0, 0))
    , offset(QPoint(0, 0))
    , scaleFactor(8)
{
    grabGesture(Qt::PinchGesture);
}

/**
 * @brief Canvas::setImage - Updates which image is being displayed on the canvas.
 * Immediately refreshes by calling `update()`
 * @param image - The current frame image to set as canvas
 */
void Canvas::setImage(QImage *image)
{
    imageToDisplay = image;
    canvasSize = QPoint(image->width(), image->height());
    update();
}

/**
 * @brief Canvas::setScale - Updates the scale, or zoom factor, of the canvas. A scale 8 means a 1x1
 * sprite pixel will take up 8x8 pixels onscreen
 * @param newScale - New scaling factor
 */
void Canvas::setScale(float newScale)
{
    scaleFactor = newScale;
}

/**
 * @brief Canvas::setOffset - Updates the offset position of the canvas for panning. (0, 0) will put the top-left
 * corner of the displayed image in the top-left corner of the frame
 * @param newOffset - New offset factor
 */
void Canvas::setOffset(QPoint newOffset)
{
    offset = newOffset;
}

/**
 * @brief Canvas::update - refresh our rendering of the frame. If any changes have been made to offset,
 * scaleFactor, or the image being displayed, this method needs to be called before
 * you'll see anything appear on screen
 */
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

/**
 * @brief Canvas::canvasToSpriteSpace - Maps a coordinate in canvas space (meaning the top left position of the canvas
 * viewport frame is (0, 0) and the bottom right is (canvas.width, canvas.height))
 * to the respective coordinates of the sprite being rendered in the canvas
 *
 * So for example, if the mouse is over a certain red pixel in the sprite, this method
 * will return the QPoint where y is the row and x is the column where that red pixel
 * is stored in the original sprite grid, if given the mouse position in the canvas
 * @param canvasSpace - The point on the canvas to scale with
 * @return Returns the QPoint stored relative in the original sprite grid
 */
QPoint Canvas::canvasToSpriteSpace(QPoint canvasSpace)
{
    QPoint relative = canvasSpace - offset;
    float x = float(relative.x()) / float(scaleFactor);
    float y = float(relative.y()) / float(scaleFactor);

    return QPoint(floor(x), floor(y));
}

/**
 * @brief Canvas::mousePressEvent - Called when a mousePressEvent is detected and emits the `canvasMousePressed()`
 * that maps the mouse position to sprite coordinates, for use by the controller
 * @param event
 */
void Canvas::mousePressEvent(QMouseEvent *event)
{
    emit canvasMousePressed(canvasToSpriteSpace(event->pos()));
}

/**
 * @brief Canvas::mouseMoveEvent - Called when a mouseMoveEvent is detected and emits the `canvasMouseMoved()`
 * that maps the mouse position to sprite coordinates, for use by the controller
 * @param event
 */
void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    emit canvasMouseMoved(canvasToSpriteSpace(event->pos()));
}

/**
 * @brief Canvas::mouseReleaseEvent - Called when a mouseReleaseEvent is detected and emits the `canvasMouseReleased()`
 * that maps the mouse position to sprite coordinates, for use by the controller
 * @param event
 */
void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    emit canvasMouseReleased(canvasToSpriteSpace(event->pos()));
}

/**
 * @brief Canvas::wheelEvent - Called when the user inputs to a horizontal or vertical scroll action to pan the
 * canvas around. Touchpad scrolling is included in that
 * @param event
 */
void Canvas::wheelEvent(QWheelEvent *event)
{
    this->setOffset(offset + event->pixelDelta());
    this->update();
}

/**
 * @brief Canvas::gestureEvent - Captures and processes gesture events, which are touch inputs. Really just serves
 * to route gestures of different types to their proper method
 *
 **Note:** `Canvas::event()` will only recognize a gesture if you use
 **`grabGesture(GestureType)` in the canvas constructor, as per Qt's funkiness
 * @param event
 */
void Canvas::gestureEvent(QGestureEvent *event)
{
    if (QGesture *eventGesture = event->gesture(Qt::PinchGesture)) {
        auto pinch = static_cast<QPinchGesture *>(eventGesture);
        pinchEvent(pinch);
    }
}

/**
 * @brief Canvas::pinchEvent - Processes pinch gestues for zooming. Zooms in or out at the mouse position
 * @param event
 */
void Canvas::pinchEvent(QPinchGesture *event)
{
    zoomAtPoint(event->scaleFactor(), event->centerPoint());
}

/**
 * @brief Canvas::zoomAtPoint - Updates the canvas's scaleFactor and offset so that the pixel underneath the
 * coordinate `centerPoint` when the user begins zooming stays fixed in place
 * @param scaleFactorMultiplier - Zoom scale factor
 * @param centerPoint - Point where the user begins zooming
 */
void Canvas::zoomAtPoint(float scaleFactorMultiplier, QPointF centerPoint)
{
    scaleFactor = fmin(MAX_ZOOM, scaleFactor * scaleFactorMultiplier);
    if (scaleFactor == MAX_ZOOM)
    {
        return;
    }
    QPointF centerRelative = offset - centerPoint;
    QPointF centerScaled = centerRelative * scaleFactorMultiplier;

    QPointF translation = (centerScaled - centerRelative);
    offset += translation.toPoint();

    update();
}

/**
 * @brief Canvas::keyPressEvent - Processes key-down inputs to allow the user to move the canvas around with
 * arrow keys. When shift is held down, the number of pixels moved per input changes
 * to `KEYBOARD_MOVE_PIXEL_STEP_SHIFT`
 *
 * Since only one widget can listen to keyboard inputs at a time, and the MainWindow
 * needs to listen for Undo/Redo actions, the event is caught in `MainWindow::keyPressEvent`
 * and piped here
 * @param event
 */
void Canvas::keyPressEvent(QKeyEvent *event)
{
    QPoint moveDirection = QPoint(0, 0);
    int key = event->key();

    if (key == Qt::Key_Left)
    {
        moveDirection = QPoint(1, 0);
    }
    else if (key == Qt::Key_Right)
    {
        moveDirection = QPoint(-1, 0);
    }
    else if (key == Qt::Key_Up)
    {
        moveDirection = QPoint(0, 1);
    }
    else if (key == Qt::Key_Down)
    {
        moveDirection = QPoint(0, -1);
    }
    else if (key == Qt::Key_Plus || key == Qt::Key_Equal)
    {
        zoomAtPoint(1.25, QPointF(width() / 2, height() / 2));
    }
    else if (key == Qt::Key_Minus || key == Qt::Key_Underscore)
    {
        zoomAtPoint(0.8, QPointF(width() / 2, height() / 2));
    }

    if (moveDirection.manhattanLength() != 0)
    {
        int pixelsToMove = KEYBOARD_MOVE_PIXEL_STEP;
        if (event->modifiers().testFlag(Qt::ShiftModifier))
        {
            pixelsToMove = KEYBOARD_MOVE_PIXEL_STEP_SHIFT;
        }

        offset += (moveDirection * pixelsToMove * scaleFactor);
        update();
    }
}

/**
 * @brief Canvas::event - in order to use Gestures (pinch zoom and all that) we have to override the default
 * event method :/
 *
 * Detects what type of event the user is inputting, and calls the appropriate handler
 * method for inputs of that type, passing in the event after appropriately casted
 * @param event
 * @return
 */
bool Canvas::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Gesture:
    {
        gestureEvent(static_cast<QGestureEvent *>(event));
        break;
    }
    case QEvent::MouseButtonPress:
    {
        mousePressEvent(static_cast<QMouseEvent *>(event));
        break;
    }
    case QEvent::MouseMove:
    {
        mouseMoveEvent(static_cast<QMouseEvent *>(event));
        break;
    }
    case QEvent::MouseButtonRelease:
    {
        mouseReleaseEvent(static_cast<QMouseEvent *>(event));
        break;
    }
    case QEvent::Wheel:
    {
        wheelEvent(static_cast<QWheelEvent *>(event));
        break;
    }
    default:
    {
        // event of a different type
    }
    }
    return true;
}
