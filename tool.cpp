/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * PTool Source
 *
 * File reviewed by: Brett Baxter, Allison Walker
 *
 * Brief:
 * The Tool is the base class that all tools will
 * inherit from. It has a base brush size and color, a way to set
 * it, and a virtual draw method meant to be overriden
 * by each tool.
 *
 *
*/

#include "tool.h"

/**
 * @brief Tool::Draw This method is the base method meant to be overwritten
 * @param image - the image to draw on
 * @param pos - the position on the image to draw on
 */
void Tool::draw(QImage &image, QPoint pos)
{
}

/**
 * @brief Pen::Draw - Sets the pixel color at the position to the current brush color
 * @param image - the image to draw on
 * @param pos - the position on the image to draw on
 */
void Pen::draw(QImage &image, QPoint pos)
{
    image.setPixelColor(pos.x(), pos.y(), brushColor);
}

/**
 * @brief Pen::Draw - Gets the pixel color at the position, sends a signal for the toolbelt to change the tool colors
 * @param image - the image to draw on
 * @param pos - the position on the image to draw on
 */
void Eyedrop::draw(QImage &image, QPoint pos)
{
    QColor color = image.pixelColor(pos);
    emit colorRetrieved(color);
}

/**
 * @brief Pen::Draw - Sets the pixel color at the position to the white, effectively erasing it
 * @param image - the image to draw on
 * @param pos - the position on the image to draw on
 */
void Eraser::draw(QImage &image, QPoint pos)
{
    // paint it white to erase!
    image.setPixelColor(pos.x(), pos.y(), QColor("white"));
}

/**
 * @brief Pen::Draw - Changes the image color to the bucket color
 * @param image - the image to draw on
 * @param pos - the position on the image to draw on
 */
void Bucket::draw(QImage &image, QPoint pos)
{
    image.fill(brushColor);
}

/**
 * @brief Tool::SetBrushSettings - Changes the tool settings
 * @param size - The new size for the brush
 * @param color - The new color for the brush
 */
void Tool::setBrushSettings(int size, QColor color)
{
    brushSize = size;
    brushColor = color;
}
