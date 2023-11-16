/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author(s): Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * PTool Header
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

#ifndef TOOL_H
#define TOOL_H

#include <QColor>
#include <QImage>
#include <QMouseEvent>
#include <QObject>

/// Base class for all tools
class Tool : public QObject
{
    Q_OBJECT
public:
    /// Brush size and color for the tool
    int brushSize;
    QColor brushColor;

    /// Default constructor, brush size is 1 at index 0, color is black
    Tool()
        : brushSize(0)
        , brushColor(QColor(0, 0, 0))
    {}

    /// Draw method, meant to be overriden by derivatives of tool
    virtual void draw(QImage &image, QPoint pos);

    /// Set brush settings for the tool
    void setBrushSettings(int size, QColor color);
};

/// Pen tool class
class Pen : public Tool
{
    Q_OBJECT
public:
    Pen() {}
    void draw(QImage &image, QPoint pos);
};

/// Eyedrop tool class
class Eyedrop : public Tool
{
    Q_OBJECT
public:
    Eyedrop() {}
    void draw(QImage &image, QPoint pos);
signals:
    /// Signal emitted when the color is retrieved using the Eyedrop tool
    void colorRetrieved(QColor color);
};

/// Eraser tool class
class Eraser : public Tool
{
    Q_OBJECT
public:
    Eraser() {}
    void draw(QImage &image, QPoint pos);
};

/// Bucket tool class
class Bucket : public Tool
{
    Q_OBJECT
public:
    Bucket() {}
    void draw(QImage &image, QPoint pos);
};

#endif // TOOL_H
