#include "model.h"
#include <QObject>

Model::Model(QObject *parent)
    : QObject{parent}
{}

uint Model::numFrames()
{
    return frames.size();
}

QImage &Model::getFrame(uint index)
{
    assert(frames.size() > index);
    return frames.at(index);
}

void Model::pushFrame(QImage frame)
{
    frames.push_back(frame);
}

void Model::insertFrame(QImage frame, uint index)
{
    frames.insert(frames.begin() + index, frame);
}

void Model::deleteFrame(uint index)
{
    assert(frames.size() > index);
    frames.erase(frames.begin() + index);
}

void Model::popFrame()
{
    frames.pop_back();
}
