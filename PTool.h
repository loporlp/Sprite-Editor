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

#ifndef PTOOL_H
#define PTOOL_H

#include <QObject>
#include <QColor>
#include <QMouseEvent>
#include <QImage>

class PTool: public QObject{
    Q_OBJECT
public:
    int brushSize;
    QColor brushColor;
    // Default constructor, brush size is 1 at index 0, color is black.
    PTool() : brushSize(0), brushColor(QColor(0,0,0)) {}
    virtual void Draw(QImage &image, QPoint pos);
    void SetBrushSettings(int size, QColor color);
};

class PPen : public PTool{
    Q_OBJECT
public:
    PPen() {}
    void Draw(QImage &image, QPoint pos);
};

class PEyedropper : public PTool{
    Q_OBJECT
public:
    PEyedropper() {}
    void Draw(QImage &image, QPoint pos);
signals:
    void colorRetrieved(QColor color);
};

class PEraser : public PTool{
    Q_OBJECT
public:
    PEraser() {}
    void Draw(QImage &image, QPoint pos);
};

class PBucket : public PTool{
    Q_OBJECT
public:
    PBucket() {}
    void Draw(QImage &image, QPoint pos);
};

#endif // PTOOL_H
