/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author: Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * PTool Header
 *
 * Brief:
 * The PTool is the base class that all tools will
 * inherit from. It has a base brush size and color, a way to set
 * it, and a virtual draw method meant to be overriden
 * by each tool.
 *
*/

#ifndef TOOL_H
#define TOOL_H

#include <QColor>
#include <QImage>
#include <QMouseEvent>
#include <QObject>

class Tool : public QObject
{
    Q_OBJECT
public:
    int brushSize;
    QColor brushColor;
    // Default constructor, brush size is 1 at index 0, color is black.
    Tool()
        : brushSize(0)
        , brushColor(QColor(0, 0, 0))
    {}
    // Draw method, meant to be overriden by derivatives of tool.
    virtual void draw(QImage &image, QPoint pos);
    void setBrushSettings(int size, QColor color);
};

class Pen : public Tool
{
    Q_OBJECT
public:
    Pen() {}
    void draw(QImage &image, QPoint pos);
};

class Eyedrop : public Tool
{
    Q_OBJECT
public:
    Eyedrop() {}
    void draw(QImage &image, QPoint pos);
signals:
    void colorRetrieved(QColor color);
};

class Eraser : public Tool
{
    Q_OBJECT
public:
    Eraser() {}
    void draw(QImage &image, QPoint pos);
};

class Bucket : public Tool
{
    Q_OBJECT
public:
    Bucket() {}
    void draw(QImage &image, QPoint pos);
};

#endif // TOOL_H
