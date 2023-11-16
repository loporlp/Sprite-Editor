/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): Allison Walker
 * Course: CS 3505
 * Fall 2023
 *
 * MainWindow Source
 *
 * File reviewed by: Brett Baxter, Allison Walker
 *
 * Brief: The main window sets up the
 * programs front end UI and button related actions.
 *
 *
*/

#include <QApplication>
#include <QColor>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    grabKeyboard();

    // Connect signals and slots for files
    connectFileActions();
    // Connect signals and slots for tools
    connectToolButtons();
    // Connect signals and slots for frames
    connectFrameButtons();
    // Connect signals and slots for animation
    connectAnimationButtons();
    // Set up Animation preview screen
    initializeAnimationPreview();
}

/**
 * @brief MainWindow::canvas - Getter for our current canvas
 * @return
 */
Canvas *MainWindow::canvas()
{
    return this->ui->canvas;
}

/**
 * @brief MainWindow::~MainWindow - Destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::connectToolButtons - Connects tool buttons to their respective slots
 */
void MainWindow::connectToolButtons()
{
    connect(ui->penButton, &QPushButton::released, this, [this]() {
        emit selectActiveTool(ToolType::Pen);
        highlightSelectedTool(ui->penButton);
    });

    connect(ui->eraserButton, &QPushButton::released, this, [this]() {
        emit selectActiveTool(ToolType::Eraser);
        highlightSelectedTool(ui->eraserButton);
    });

    connect(ui->eyedropButton, &QPushButton::released, this, [this]() {
        emit selectActiveTool(ToolType::Eyedrop);
        highlightSelectedTool(ui->eyedropButton);
    });

    connect(ui->bucketButton, &QPushButton::released, this, [this]() {
        emit selectActiveTool(ToolType::Bucket);
        highlightSelectedTool(ui->bucketButton);
    });

    // Connect color buttons to their respective slots
    connect(ui->colorButton, &QPushButton::released, this, &MainWindow::colorButtonPressed);
    connect(ui->selectedColorButton, &QPushButton::released, this, &MainWindow::colorButtonPressed);

    // Connect other tool-related buttons to their respective slots
    connect(ui->undoButton, &QPushButton::released, this, &MainWindow::undoButtonPressed);
    connect(ui->redoButton, &QPushButton::released, this, &MainWindow::redoButtonPressed);   
    connect(ui->brushSizeBox, &QComboBox::currentIndexChanged, this, &MainWindow::brushSizeChanged);
}

/**
 * @brief MainWindow::connectFrameButtons - Connect frame-related buttons to their respective slots
 */
void MainWindow::connectFrameButtons()
{
    connect(ui->addFrameButton, &QPushButton::released, this, &MainWindow::addFrameButtonPressed);
    connect(ui->deleteFrameButton,
            &QPushButton::released,
            this,
            &MainWindow::deleteFrameButtonPressed);
    connect(ui->moveFrameUpButton,
            &QPushButton::released,
            this,
            &MainWindow::moveFrameUpButtonPressed);
    connect(ui->moveFrameDownButton,
            &QPushButton::released,
            this,
            &MainWindow::moveFrameDownButtonPressed);

    connect(ui->frameListWidget, &QListWidget::itemClicked, this, &MainWindow::frameSelected);
    connect(ui->frameListWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        emit setFrameToEdit(item->data(0).toInt());
    });
}

/**
 * @brief MainWindow::connectAnimationButtons - Connect animation-related buttons to their respective slots
 */
void MainWindow::connectAnimationButtons()
{
    connect(ui->playButton, &QPushButton::released, this, [this]() { emit startAnimation(true); });
    connect(ui->pauseButton, &QPushButton::released, this, [this]() { emit startAnimation(false); });
    connect(ui->fpsSlider, &QSlider::valueChanged, this, &MainWindow::fpsSliderChanged);
}

/**
 * @brief MainWindow::connectFileActions - Connect file-related actions to their respective slots
 */
void MainWindow::connectFileActions()
{
    connect(ui->saveFileAction, &QAction::triggered, this, &MainWindow::saveFileAction);
    connect(ui->newFileAction, &QAction::triggered, this, &MainWindow::newFileAction);
    connect(ui->openFileAction, &QAction::triggered, this, &MainWindow::openFileAction);
    connect(ui->resizeCanvasAction, &QAction::triggered, this, &MainWindow::sizeCanvasAction);
}

//-----Tool updates-----//

/**
 * @brief MainWindow::colorButtonPressed - Actions for when colorButton is pressed
 */
void MainWindow::colorButtonPressed()
{
    // Open a color dialog and emit the selected color
    QColor color = QColorDialog::getColor();
    emit setPenColor(color);

    currentColor = color;

    // Set the background color of the selected color button
    const QString setColor("QPushButton { background-color : %1; }");
    ui->selectedColorButton->setStyleSheet(setColor.arg(color.name()));
    ui->selectedColorButton->update();
}

/**
 * @brief MainWindow::brushSizeChanged - Emits the brush size and color when the brush size is changed
 */
void MainWindow::brushSizeChanged()
{
    emit selectBrushSettings(ui->brushSizeBox->currentIndex(), currentColor);
}

/**
 * @brief MainWindow::recieveNewColor - Update the current color and the background color of the selected color button
 * @param color
 */
void MainWindow::recieveNewColor(QColor color)
{
    currentColor = color;

    const QString setColor("QPushButton { background-color : %1; }");
    ui->selectedColorButton->setStyleSheet(setColor.arg(color.name()));
    ui->selectedColorButton->update();
}

/**
 * @brief MainWindow::highlightSelectedTool - Highlights the active tool button when clicked
 * @param button
 */
void MainWindow::highlightSelectedTool(QPushButton *button)
{
    // Reset the styleSheet for all tool buttons
    ui->penButton->setStyleSheet("");
    ui->eraserButton->setStyleSheet("");
    ui->eyedropButton->setStyleSheet("");
    ui->bucketButton->setStyleSheet("");

    // Highlight the clicked tool button
    button->setStyleSheet("background-color: #d9d9d8");

    brushSizeChanged();
}

/**
 * @brief MainWindow::undoButtonPressed - Emit the undo action signal
 */
void MainWindow::undoButtonPressed()
{
    emit undoAction();
}

/**
 * @brief MainWindow::updateCanvas - Update the canvas with the provided image
 * @param image
 */
void MainWindow::updateCanvas(QImage image)
{
    canvas()->setImage(&image);
}

/**
 * @brief MainWindow::redoButtonPressed - Emit the redo action signal
 */
void MainWindow::redoButtonPressed()
{
    emit redoAction();
}

//-----Animation updates-----//

/**
 * @brief MainWindow::initializeAnimationPreview - Initialize the animation preview with an empty frame
 */
void MainWindow::initializeAnimationPreview()
{
    QPixmap p = QPixmap(ui->animationScreen->size());
    p.fill(QColor(Qt::white));
    ui->animationScreen->setPixmap(p);

    addFrameToList();
}

/**
 * @brief MainWindow::playAnimation - Display the animation by updating the animation screen with the current frame
 * @param frameImage
 */
void MainWindow::playAnimation(const QImage &frameImage)
{
    QPixmap p;
    if (ui->actualSizeCheckBox->isChecked()) {
        p = QPixmap::fromImage(frameImage);
    } else {
        p = QPixmap::fromImage(frameImage.scaled(ui->animationScreen->size(), Qt::KeepAspectRatio));
    }
    ui->animationScreen->setPixmap(p);
}

/**
 * @brief MainWindow::fpsSliderChanged - Handle the change in FPS (frames per second) by updating the FPS label and emitting the new FPS value
 * @param value
 */
void MainWindow::fpsSliderChanged(int value)
{
    ui->fpsLabel->setText(QString("%1 FPS").arg(value));
    emit setFPS(value);
}

/**
 * @brief MainWindow::receiveAnimationFrameData - Receive animation frame data and play the frame with a delay
 * @param frame
 * @param delay
 */
void MainWindow::receiveAnimationFrameData(QImage frame, int delay)
{
    QTimer::singleShot(delay, this, [this, frame]() { playAnimation(frame); });
}

//-----Frame updates-----//

/**
 * @brief MainWindow::sizeCanvasAction - Handle the action of resizing the canvas by prompting the user for a new canvas size and emitting the new size
 */
void MainWindow::sizeCanvasAction()
{

    int size = QInputDialog::getInt(this, "Canvas Size", "Set Canvas Size", 64, 8, 128, 8);
    emit resizeCanvas(size, size);
}

/**
 * @brief MainWindow::addFrameToList - Add a new frame to the frame list and set it as the current item
 */
void MainWindow::addFrameToList()
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setData(0, frameList.size());
    ui->frameListWidget->addItem(item);
    ui->frameListWidget->setCurrentItem(item);
    frameList.append(item);
}

/**
 * @brief MainWindow::addFrameButtonPressed - Handle the action of adding a new frame by adding it to the list and emitting the signal to add a frame
 */
void MainWindow::addFrameButtonPressed()
{
    addFrameToList();
    emit addFrame();
}

/**
 * @brief MainWindow::deleteFrameButtonPressed -  Handle the action of deleting a frame by removing it from the list and emitting the signal to delete a frame
 */
void MainWindow::deleteFrameButtonPressed()
{
    int id = ui->frameListWidget->currentItem()->data(0).toInt();
    if (frameList.size() == 1) {
        return;
    }

    delete frameList[id];

    for (int i = id; i < frameList.size() - 1; i++) {
        frameList[i] = frameList[i + 1];
        frameList[i]->setData(0, i);
    }
    frameList.pop_back();

    emit deleteFrame();

    // Handles the case where we are deleting the first frame in the list
    if (id == 0) {
        ui->frameListWidget->setCurrentRow(0);
    } else {
        ui->frameListWidget->setCurrentRow(id - 1);
    }
}

/**
 * @brief MainWindow::moveFrameUpButtonPressed - Handle the action of moving a frame up by emitting the signal to move the frame up and updating the current row
 */
void MainWindow::moveFrameUpButtonPressed()
{
    int id = ui->frameListWidget->currentItem()->data(0).toInt();
    if (id == 0)
        return;
    emit moveFrame(id - 1, id);
    ui->frameListWidget->setCurrentRow(id - 1);
    emit setFrame(id - 1);
}

/**
 * @brief MainWindow::moveFrameDownButtonPressed - Handle the action of moving a frame down by emitting the signal to move the frame down and updating the current row
 */
void MainWindow::moveFrameDownButtonPressed()
{
    int id = ui->frameListWidget->currentItem()->data(0).toInt();
    if (id == frameList.size() - 1)
        return;
    emit moveFrame(id, id + 1);
    ui->frameListWidget->setCurrentRow(id + 1);
    emit setFrame(id + 1);
}

/**
 * @brief MainWindow::frameSelected - Handle the action of selecting a frame by emitting the signal to set the current frame
 */
void MainWindow::frameSelected()
{
    int id = ui->frameListWidget->currentItem()->data(0).toInt();
    emit setFrame(id);
}

//-----File updates-----//

/**
 * @brief MainWindow::saveFileAction - Handle the action of saving a file by prompting the user for a file directory and emitting the signal to save the file
 */
void MainWindow::saveFileAction()
{
    QString fileDirectory = QFileDialog::getSaveFileName(this,
                                                         tr("Choose Directory"),
                                                         "C://",
                                                         "Sprite Pixel Image (*.ssp);;");
    changed = false;
    emit saveFile(QString(fileDirectory));
}

/**
 * @brief MainWindow::openFileAction - Handle the action of opening a file by prompting the user for a file directory and emitting the signal to load the file
 */
void MainWindow::openFileAction()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    "C://",
                                                    "Sprite Pixel Image (*.ssp);;");
    frameList.clear();
    ui->frameListWidget->clear();
    addFrameToList();
    emit loadFile(QString(filename));
}

/**
 * @brief MainWindow::newFileAction - Handle the action of creating a new file by clearing the frame list and emitting the signal to create a new file
 */
void MainWindow::newFileAction()
{
    frameList.clear();
    ui->frameListWidget->clear();
    addFrameToList();
    emit newFile();
}

/**
 * @brief MainWindow::addFramesToList - Add a specified number of frames to the frame list
 * @param count
 */
void MainWindow::addFramesToList(int count)
{
    for (int i = 0; i < count; i++) {
        addFrameToList();
    }
}

/**
 * @brief MainWindow::keyPressEvent - Key press events
 * @param event
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    bool controlIsDown = event->modifiers().testFlag(Qt::ControlModifier);
    bool shiftIsDown = event->modifiers().testFlag(Qt::ShiftModifier);

    if (controlIsDown)
    {
        if (key == Qt::Key_Z)
        {
            if (shiftIsDown)
            {
                emit redoAction(); // Ctrl + Shift + Z: Redo
            }
            else
            {
                emit undoAction(); // Ctrl + Z: Undo
            }
        }
        else if (key == Qt::Key_Y)
        {
            emit redoAction(); // Ctrl + Y: Redo
        }
        else if (key == Qt::Key_S)
        {
            saveFileAction(); // Ctrl + S: Save File
        }
        else if (key == Qt::Key_O)
        {
            openFileAction(); // Ctrl + O: Open File
        }
        else if (key == Qt::Key_N)
        {
            newFileAction(); // Ctrl + N: New File
        }
    }
    else
    {
        if (key == Qt::Key_B)
        {
            emit selectActiveTool(ToolType::Pen); // B: Brush
            highlightSelectedTool(ui->penButton);
        }
        else if (key == Qt::Key_E)
        {
            emit selectActiveTool(ToolType::Eraser); // E: Eraser
            highlightSelectedTool(ui->eraserButton);
        }
        else if (key == Qt::Key_I)
        {
            emit selectActiveTool(ToolType::Eyedrop); // I: Eyedropper
            highlightSelectedTool(ui->eyedropButton);
        }
        else if (key == Qt::Key_F)
        {
            emit selectActiveTool(ToolType::Bucket); // F: Bucket (Fill)
            highlightSelectedTool(ui->bucketButton);
        }
        else if (key == Qt::Key_Space)
        {
            emit toggleAnimation();
        }
        // The keys being pressed aren't relevant here,
        // but maybe it'll be something the canvas cares about
        canvas()->keyPressEvent(event);
    }
}
