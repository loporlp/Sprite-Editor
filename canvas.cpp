#include "canvas.h"
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
    , imageHolder{new QLabel(this)}
    , canvasSize(QVector2D(0, 0))
    , scaleFactor(5)
{
    imageHolder->setAlignment(Qt::AlignLeft);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(imageHolder);
    this->setLayout(layout);

    this->setStyleSheet("QWidget {background-color: rgb(200, 255, 255)}");
}

void Canvas::setImage(QImage *image)
{
    imageToDisplay = image;
    canvasSize = QVector2D(image->width(), image->height());
    update();
}

void Canvas::update()
{
    QImage scaled = imageToDisplay->scaled(canvasSize.x() * scaleFactor,
                                           canvasSize.y() * scaleFactor,
                                           Qt::KeepAspectRatio);

    imageHolder->setPixmap(QPixmap::fromImage(scaled));
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    emit canvasMousePressed(canvasToSpriteSpace(event->pos()));
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    emit canvasMouseMoved(canvasToSpriteSpace(event->pos()));
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    emit canvasMouseReleased(canvasToSpriteSpace(event->pos()));
}

QPoint Canvas::canvasToSpriteSpace(QPoint canvasSpace)
{
    return canvasSpace / scaleFactor;
}
