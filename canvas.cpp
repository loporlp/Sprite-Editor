#include "canvas.h"
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
    , canvasSize(QVector2D(0, 0))
    , imageHolder{new QLabel(this)}
{
    imageHolder->setAlignment(Qt::AlignCenter);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(imageHolder);
    this->setLayout(layout);

    this->setStyleSheet("QWidget {background-color: rgb(200, 255, 255)}");
}

void Canvas::setImage(QImage image)
{
    imageToDisplay = image;
    updateDisplayTransform();
}

void Canvas::updateDisplayTransform()
{
    QImage scaled = imageToDisplay.scaled(imageHolder->width(), imageHolder->height());
    imageHolder->setPixmap(QPixmap::fromImage(scaled));
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "clicked: " << event->pos().x() << ", " << event->pos().y();
    emit canvasMousePressed(canvasToSpriteSpace(event->pos()));
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "moved: " << event->pos().x() << ", " << event->pos().y();
    emit canvasMouseMoved(canvasToSpriteSpace(event->pos()));
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "released: " << event->pos().x() << ", " << event->pos().y();
    emit canvasMouseReleased(canvasToSpriteSpace(event->pos()));
}

QPoint Canvas::canvasToSpriteSpace(QPoint canvasSpace)
{
    // NOTE: currently assumes zoom is the imageholder width/height - change to zoom factor when implemented
    return QPoint(canvasSpace.x() / imageHolder->width() / 100,
                  canvasSpace.y() / imageHolder->width() / 100);
}
