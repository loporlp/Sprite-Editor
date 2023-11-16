/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): David Cosby, Andrew Wilhelm, Allison Walker,
 * Mason Sansom, AJ Kennedy, Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * MainWindow Header
 *
 * File reviewed by: Brett Baxter, Allison Walker
 *
 * Brief:
 * The main window sets up the
 * programs front end UI and button related actions.
 *
 *
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QTimer>

#include "enums.h"
#include "canvas.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /// Constructor for the MainWindow class
    explicit MainWindow(QWidget *parent = nullptr);

    /// Destructor for the MainWindow class
    ~MainWindow();

    /// Accessor for the canvas
    Canvas *canvas();

    /// Canvas update methods
    void updateCanvas(QImage image);
    void addFramesToList(int count);

signals:
    /// Tool related signals
    void selectActiveTool(ToolType tool);
    void selectBrushSettings(int size, QColor &color);
    void setPenColor(const QColor &color);

    /// Undo and redo signals
    void undoAction();
    void redoAction();

    /// Frame related signals
    void setFrameToEdit(int frameIndex);
    void addFrame();
    void deleteFrame();
    void moveFrame(int fromIndex, int toIndex);
    void resizeCanvas(int width, int height);
    void setFrame(int frameIndex);

    /// Animation related signals
    void startAnimation(bool play);
    void toggleAnimation();
    void setFPS(int fps);

    /// File related signals
    void saveFile(const QString &filePath);
    void loadFile(const QString &filePath);
    void newFile();

public slots:
    /// Animation related Slot
    void playAnimation(const QImage &frameImage);

private slots:
    /// Tool related slots
    void colorButtonPressed();
    void brushSizeChanged();

    /// Undo and redo slots
    void undoButtonPressed();
    void redoButtonPressed();

    /// Animation related slots
    void fpsSliderChanged(int value);

    /// Frame related slots
    void sizeCanvasAction();
    void addFrameToList();
    void addFrameButtonPressed();
    void deleteFrameButtonPressed();
    void moveFrameUpButtonPressed();
    void moveFrameDownButtonPressed();
    void frameSelected();

    /// File related slots
    void saveFileAction();
    void openFileAction();
    void newFileAction();

public slots:
    void recieveNewColor(QColor color);
    void receiveAnimationFrameData(QImage frame, int delay);
    
private:
    Ui::MainWindow *ui;

    QList<QListWidgetItem *> frameList;

    QImage image;
    bool actualSize;
    bool changed;

    /// Initialization method for animation preview
    void initializeAnimationPreview();

    /// Connection methods for various buttons
    void connectToolButtons();
    void highlightSelectedTool(QPushButton *button);
    void connectFrameButtons();
    void connectFileActions();
    void connectAnimationButtons();

    /// Current color variable
    QColor currentColor = (QColor(Qt::black));

protected:
    /// helper method to draw when a mouse occurs
    void drawOnEvent(QMouseEvent *event);

    /// Key press event
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
