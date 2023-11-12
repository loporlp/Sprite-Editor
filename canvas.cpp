#include "canvas.h"
#include <QLabel>
#include <QMouseEvent>
#include <QStyle>
#include <QTransform>
#include <QVBoxLayout>

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
    , imageHolder{new QLabel(this)}
    , canvasSize(QPoint(0, 0))
    , offset(QPoint(50, 0))
    , scaleFactor(8)
{
    this->setStyleSheet("QWidget {background-color: rgb(200, 255, 255)}");
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
    QTransform imageTransformation;
    imageTransformation.scale(scaleFactor, scaleFactor);

    imageHolder->setPixmap(QPixmap::fromImage(*imageToDisplay).transformed(imageTransformation));
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

QPoint Canvas::canvasToSpriteSpace(QPoint canvasSpace)
{
    return canvasSpace / scaleFactor;
}
