/*
 * Assignment 8: Pixel Image Software Suite (PISS)
 * Class Author: Brett Baxter
 * Course: CS 3505
 * Fall 2023
 *
 * PTool Source
 *
 * Brief:
 * The PTool is the base class that all tools will
 * inherit from. It has a base brush size and color, a way to set
 * it, and a virtual draw method meant to be overriden
 * by each tool.
 *
*/

#include "PTool.h"
#include <QDebug>

void PTool::Draw(QImage &image, QPoint pos){
    qDebug() << "PTool Draw";
}

void PPen::Draw(QImage &image, QPoint pos){
    image.setPixelColor(pos.x(), pos.y(), brushColor);

}

void PEyedropper::Draw(QImage &image, QPoint pos){
    QColor color = image.pixelColor(pos);
    emit colorRetrieved(color);
}

void PEraser::Draw(QImage &image, QPoint pos){
    // paint it white to erase!
    image.setPixelColor(pos.x(), pos.y(), QColor("white"));

}

void PBucket::Draw(QImage &image, QPoint pos){
    image.fill(brushColor);
}

void PTool::SetBrushSettings(int size, QColor color){
    brushSize = size;
    brushColor = color;
}
