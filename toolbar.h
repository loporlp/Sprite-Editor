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

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QMouseEvent>
#include <QObject>
#include "enums.h"
#include "tool.h"

class ToolBar : public QObject
{
    Q_OBJECT
private:
    // Test tool, will be replaced by other classes that inherit from PTool
    Tool tool;
    Pen pen;
    Eraser eraser;
    Eyedrop eyedropper;
    Bucket bucket;
    // Pointer to the current tool.
    Tool *currentTool;

public:
    // Constructor, by default the current tool is the pen tool.
    ToolBar()
        : currentTool(&pen)
    {
        connect(&eyedropper, &Eyedrop::colorRetrieved, this, &ToolBar::setPenBrushColor);
    }
    // Returns a pointer to the current tool.
    Tool *CurrentTool();
public slots:
    void drawWithCurrentTool(QImage &image, QPoint pos);
    void updateCurrentTool(ToolType tool);
    void setCurrentBrushSettings(int size, QColor color);
    void setPenBrushColor(QColor color);

signals:
    void toolChanged();
    void canvasChanged();
    void colorChanged(QColor color);
};

#endif // TOOLBAR_H
