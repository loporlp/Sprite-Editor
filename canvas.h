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
    QVector2D canvasSize;
    Q_OBJECT
    QLabel *imageHolder;
    QImage imageToDisplay;

public:
    explicit Canvas(QWidget *parent = nullptr);
    void setImage(QImage image);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void updateDisplayTransform();
    QPoint canvasToSpriteSpace(QPoint canvasSpace);

signals:
    void canvasMousePressed(QPoint spriteMouseLocation);
    void canvasMouseMoved(QPoint spriteMouseLocation);
    void canvasMouseReleased(QPoint spriteMouseLocation);
};

#endif // CANVAS_H
