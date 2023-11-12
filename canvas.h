#ifndef CANVAS_H
#define CANVAS_H

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
    QVector2D canvasSize;
    float scaleFactor;
    QImage *imageToDisplay;

public:
    explicit Canvas(QWidget *parent = nullptr);
    void setImage(QImage *image);
    void setScale(float);
    void update();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QPoint canvasToSpriteSpace(QPoint canvasSpace);

signals:
    void canvasMousePressed(QPoint spriteMouseLocation);
    void canvasMouseMoved(QPoint spriteMouseLocation);
    void canvasMouseReleased(QPoint spriteMouseLocation);
};

#endif // CANVAS_H
