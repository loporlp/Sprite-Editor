/*
 * CS3505 HW 8
 *
 * A8: Sprite Editor Implementation
 *
 * Andrew Wilhelm, Allison Walker,
 * AJ Kennedy, Brett Baxter,
 * David Cosby, Mason Sansom
 *
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>

#include "ui_mainwindow.h"
#include "canvas.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    // Constructor for the MainWindow class
    explicit MainWindow(QWidget *parent = nullptr);

    // Destructor for the MainWindow class
    ~MainWindow();

    // Tool enums
    enum Tool{ Pen, Eraser, Fill, Eyedrop, Bucket };
    Canvas *canvas();

    void updateCanvas(QImage image);

signals:
    // Tool related signals
    void selectActiveTool(Tool tool);
    void setPenColor(const QColor &color);
    void undoAction();
    void redoAction();

    // Frame related signals
    void setFrameToEdit(int frameIndex);
    void addFrame();
    void deleteFrame();
    void moveFrame(int fromIndex, int toIndex);
    void resizeCanvas(int width, int height);

    // Animation related signals
    void startAnimation(bool play);
    void setFPS(int fps);

    // File related signals
    void saveFile(const QString &filePath);
    void loadFile(const QString &filePath);

private slots:
    // Tool related slots
    void colorButtonPressed();
    void undoButtonPressed();
    void redoButtonPressed();

    // Animation related slots
    void playAnimation(const QImage &frameImage);
    void fpsSliderChanged(int value);

    // Frame related slots
    void updateFrameEditor(const QImage &frameImage, int editingTarget);
    void sizeCanvasAction();
    void addFrameToList();
    void addFrameButtonPressed();
    void deleteFrameButtonPressed();
    void moveFrameUpButtonPressed();
    void moveFrameDownButtonPressed();

    // File related slots
    void saveFileAction();
    void openFileAction();
    void newFileAction();
    
private:
    Ui::MainWindow *ui;

    QList<QListWidgetItem *> frameList;

    //QPixmap canvas;

    QImage image;

    bool actualSize;
    bool changed;

    void initializeAnimationPreview();

    void connectToolButtons();
    void connectFrameButtons();
    void connectFileActions();
};

#endif // MAINWINDOW_H
