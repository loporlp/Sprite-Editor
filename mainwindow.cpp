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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set up Animation preview screen
    initializeAnimationPreview();

    // Connect signals and slots for tools
    connectToolButtons();
    connect(ui->colorButton, &QPushButton::released, this, &MainWindow::colorButtonPressed);
    connect(ui->undoButton, &QPushButton::released, this, &MainWindow::undoButtonPressed);
    connect(ui->redoButton, &QPushButton::released, this, &MainWindow::redoButtonPressed);

    // Connect signals and slots for frames
    //connect(ui->frameListWidget, &QListWidget::itemClicked, this, &MainWindow::setFrameToEdit);
    connect(ui->frameListWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem * item){ emit setFrameToEdit(item->data(0).toInt());});

    connectFrameButtons();

    // Connect signals and slots for animation
    connect(ui->playButton, &QPushButton::released, this, [this]() { emit startAnimation(true); });
    connect(ui->pauseButton, &QPushButton::released, this, [this]() { emit startAnimation(false); });

    connect(ui->fpsSlider, &QSlider::valueChanged, this, &MainWindow::fpsSliderChanged);

    connect(ui->resizeCanvasAction, &QAction::triggered, this, &MainWindow::sizeCanvasAction);
    connect(ui->actualSizeCheckBox, &QCheckBox::clicked, this, [this]() { emit startAnimation(true); });

    // Connect signals and slots for files
    connectFileActions();
}

Canvas *MainWindow::canvas()
{
    return this->ui->canvas;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Connecting signals and slots
void MainWindow::connectToolButtons()
{
    connect(ui->penButton, &QPushButton::released, this, [this]() { emit selectActiveTool(Pen); });
    connect(ui->eraserButton, &QPushButton::released, this, [this]() { emit selectActiveTool(Eraser); });
    connect(ui->eyedropButton, &QPushButton::released, this, [this]() { emit selectActiveTool(Eyedrop); });
    connect(ui->bucketButton, &QPushButton::released, this, [this]() { emit selectActiveTool(Bucket); });
}

void MainWindow::connectFrameButtons()
{
    connect(ui->addFrameButton, &QPushButton::released, this, &MainWindow::addFrameButtonPressed);
    connect(ui->deleteFrameButton, &QPushButton::released, this, &MainWindow::deleteFrameButtonPressed);
    connect(ui->moveFrameUpButton, &QPushButton::released, this, &MainWindow::moveFrameUpButtonPressed);
    connect(ui->moveFrameDownButton, &QPushButton::released, this, &MainWindow::moveFrameDownButtonPressed);
}

void MainWindow::connectFileActions()
{
    connect(ui->saveFileAction, &QAction::triggered, this, &MainWindow::saveFileAction);
    connect(ui->newFileAction, &QAction::triggered, this, &MainWindow::newFileAction);
    connect(ui->openFileAction, &QAction::triggered, this, &MainWindow::openFileAction);
}

//-----Tool updates-----//
void MainWindow::colorButtonPressed()
{
    QColor color = QColorDialog::getColor();
    emit setPenColor(color);
}

void MainWindow::undoButtonPressed()
{
    emit undoAction();
}

void MainWindow::updateCanvas(QImage* image)
{
    canvas()->setImage(image);
}

void MainWindow::redoButtonPressed()
{
    // Implement redo functionality
}

//-----Animation updates-----//
void MainWindow::initializeAnimationPreview()
{
    QPixmap p = QPixmap(ui->animationScreen->size());
    p.fill(QColor(Qt::white));
    ui->animationScreen->setPixmap(p);
    ui->frameListWidget->setIconSize(QSize(50, 50));
    addFrameToList();
}

void MainWindow::playAnimation(const QImage &frameImage)
{
    QPixmap p;
    if (actualSize) {
        p = QPixmap::fromImage(frameImage);
    } else {
        p = QPixmap::fromImage(frameImage.scaled(ui->animationScreen->size(), Qt::KeepAspectRatio));
    }
    ui->animationScreen->setPixmap(p);
}

void MainWindow::fpsSliderChanged(int value)
{
    ui->fpsLabel->setText(QString("%1 FPS").arg(value));
    emit setFPS(value);
}

//-----Frame updates-----//
void MainWindow::updateFrameEditor(const QImage &frameImage, int editingTarget)
{
    image = frameImage;
    QPixmap p = QPixmap::fromImage(frameImage.scaled(QSize(50, 50), Qt::KeepAspectRatio));
    frameList.at(editingTarget)->setIcon(QIcon(p));
    update();
    changed = true;
}

void MainWindow::sizeCanvasAction()
{
    int size = QInputDialog::getInt(this, "Canvas Size", "Set Canvas Size");
    emit resizeCanvas(size, size);
}

void MainWindow::addFrameToList()
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setData(0, frameList.size());
    ui->frameListWidget->addItem(item);
    ui->frameListWidget->setCurrentItem(item);
    frameList.append(item);
}

void MainWindow::addFrameButtonPressed()
{
    addFrameToList();
    emit addFrame();
}

void MainWindow::deleteFrameButtonPressed()
{
    if (frameList.size() == 1) {
        return;
    }

    int id = ui->frameListWidget->currentItem()->data(0).toInt();

    delete frameList[id];

    for (int i = id; i < frameList.size() - 1; i++) {
        frameList[i] = frameList[i + 1];
        frameList[i]->setData(0, i);
    }
    frameList.pop_back();
    emit deleteFrame();
}

void MainWindow::moveFrameUpButtonPressed()
{
    int id = ui->frameListWidget->currentItem()->data(0).toInt();
    if (id == 0)
        return;
    emit moveFrame(id - 1, id);
    ui->frameListWidget->setCurrentRow(id - 1);
    emit setFrameToEdit(id - 1);
}

void MainWindow::moveFrameDownButtonPressed()
{
    int id = ui->frameListWidget->currentItem()->data(0).toInt();
    if (id == frameList.size() - 1)
        return;
    emit moveFrame(id, id + 1);
    ui->frameListWidget->setCurrentRow(id + 1);
    emit setFrameToEdit(id + 1);
}

//-----File updates-----//
void MainWindow::saveFileAction()
{
    QString fileDirectory = QFileDialog::getSaveFileName(
        this,
        tr("Choose Directory"),
        "C://",
        "Sprite Pixel Image (*.ssp);;");
    changed = false;
    emit saveFile(QString(fileDirectory));
}

void MainWindow::openFileAction()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C://",
        "Sprite Pixel Image (*.ssp);;");
    emit loadFile(filename);
}

void MainWindow::newFileAction()
{
    // Implement new file functionality
}
