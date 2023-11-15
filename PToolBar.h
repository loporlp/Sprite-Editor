/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author: Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * PToolBar Header
 *
 * Brief:
 * The PToolBar manages the different types of tools available
 * in PISS. It stores one of each type of tool and a pointer to
 * the current tool.
 *
*/

#ifndef PTOOLBAR_H
#define PTOOLBAR_H

#include <QObject>
#include <QMouseEvent>
#include "PTool.h"
#include "PEnums.h"

class PToolBar: public QObject{
    Q_OBJECT
private:
    // Test tool, will be replaced by other classes that inherit from PTool
    PTool tool;
    PPen pen;
    PEraser eraser;
    PEyedropper eyedropper;
    PBucket bucket;
    // Pointer to the current tool.
    PTool* currentTool;
public:
    // Constructor, by default the current tool is the pen tool.
    PToolBar(): currentTool(&pen) { connect(&eyedropper, &PEyedropper::colorRetrieved, this, &PToolBar::SetPenBrushColor); }
    // Returns a pointer to the current tool.
    PTool* CurrentTool();
public slots:
    // Tells the toolbar to draw with the currently selected tool, this also may change.
    // Other option: CurrentTool()->Draw();
    void DrawWithCurrentTool(QImage &image, QPoint pos);
    void UpdateCurrentTool(Tool tool);
    void SetCurrentBrushSettings(int size, QColor color);
    void SetPenBrushColor(QColor color);

signals:
    void ToolChanged();
    void CanvasChanged();
    void ColorChanged(QColor color);
};

#endif // PTOOLBAR_H
