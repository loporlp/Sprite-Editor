/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author: Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * PToolBar Source
 *
 * Brief:
 * The PToolBar manages the different types of tools available
 * in PISS. It stores one of each type of tool and a pointer to
 * the current tool.
 *
*/

#include "PToolBar.h"
#include <QDebug>

PTool *PToolBar::CurrentTool()
{
    return currentTool;
}

/**
 * @brief PToolBar::DrawWithCurrentTool
 * Draws with the currently selected tool.
 */
void PToolBar::DrawWithCurrentTool(QImage &image, QPoint pos)
{
    currentTool->Draw(image, pos);
    emit CanvasChanged();
}

/**
 * @brief PToolBar::UpdateCurrentTool
 * Changes the current tool.
 * @param tool - The new tool to set.
 */
void PToolBar::UpdateCurrentTool(Tool tool)
{
    if (tool == Tool::Pen) {
        currentTool = &pen;
        emit ColorChanged(currentTool->brushColor);
    } else if (tool == Tool::Eraser) {
        currentTool = &eraser;
        emit ColorChanged(currentTool->brushColor);
    } else if (tool == Tool::Eyedrop) {
        currentTool = &eyedropper;
        emit ColorChanged(currentTool->brushColor);
    } else if (tool == Tool::Bucket) {
        currentTool = &bucket;
        emit ColorChanged(currentTool->brushColor);
    }
    emit ToolChanged();
}

/**
 * @brief PToolBar::UpdateBrushSettings
 * Update the current brush settings.
 * @param size - The new brush size.
 * @param color - The new brush color.
 */
void PToolBar::SetCurrentBrushSettings(int size, QColor color)
{
    currentTool->SetBrushSettings(size, color);
}

void PToolBar::SetPenBrushColor(QColor color)
{
    //    currentTool->SetBrushSettings(tool.brushSize, color);
    pen.brushColor = color;
    bucket.brushColor = color;
    tool.brushColor = color;
    emit ColorChanged(color);
}
