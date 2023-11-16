/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * PToolBar Source
 *
 * File reviewed by: Brett Baxter, Allison Walker
 *
 * Brief:
 * The ToolBar manages the different types of tools available
 * in PISS. It stores one of each type of tool and a pointer to
 * the current tool.
 *
 *
*/

#include "toolbar.h"

Tool* ToolBar::CurrentTool()
{
    return currentTool;
}

/**
 * @brief PToolBar::DrawWithCurrentTool - Draws with the currently selected tool
 */
void ToolBar::drawWithCurrentTool(QImage &image, QPoint pos)
{
    currentTool->draw(image, pos);
    emit canvasChanged();
}

/**
 * @brief PToolBar::UpdateCurrentTool - Changes the current tool
 * @param tool - The new tool to set
 */
void ToolBar::updateCurrentTool(ToolType tool)
{
    if (tool == ToolType::Pen)
    {
        currentTool = &pen;
        emit colorChanged(currentTool->brushColor);
    } else if (tool == ToolType::Eraser)
    {
        currentTool = &eraser;
        emit colorChanged(currentTool->brushColor);
    } else if (tool == ToolType::Eyedrop)
    {
        currentTool = &eyedropper;
        emit colorChanged(currentTool->brushColor);
    } else if (tool == ToolType::Bucket)
    {
        currentTool = &bucket;
        emit colorChanged(currentTool->brushColor);
    }
    emit toolChanged();
}

/**
 * @brief PToolBar::UpdateBrushSettings - Update the current brush settings
 * @param size - The new brush size
 * @param color - The new brush color
 */
void ToolBar::setCurrentBrushSettings(int size, QColor color)
{
    currentTool->setBrushSettings(size, color);
}

/**
 * @brief ToolBar::SetPenBrushColor - Sets all tool colors to the param color, for use with eyedropper
 * @param color - The new brush color
 */
void ToolBar::setPenBrushColor(QColor color)
{
    pen.brushColor = color;
    bucket.brushColor = color;
    tool.brushColor = color;
    emit colorChanged(color);
}
