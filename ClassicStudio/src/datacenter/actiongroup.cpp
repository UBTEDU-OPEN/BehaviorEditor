#include "actiongroup.h"

#include <algorithm>

ActionGroup::ActionGroup(QString groupName, QSet<int> frameIndexes, QObject* parent)
    : QObject(parent)
    , groupName_(groupName)
    , frameIndexes_(frameIndexes)
{

}

int ActionGroup::groupIndex()
{
    auto frameList = frameIndexes_.toList();
    std::sort(frameList.begin(),frameList.end());
    return frameList.first();
}

int ActionGroup::lastFrameIndex()
{
    auto frameList = frameIndexes_.toList();
    std::sort(frameList.begin(),frameList.end());
    return frameList.last();
}

int ActionGroup::groupWidth()
{
    auto frameList = frameIndexes_.toList();
    std::sort(frameList.begin(),frameList.end());
    return frameList.last() - frameList.first() + 1;
}

void ActionGroup::setGroupIndex(int index)
{
    int delta = index - groupIndex();
    QSet<int> tempIndexes;
    for(auto frameIndex : frameIndexes_) {
        tempIndexes.insert(frameIndex+delta);
    }
    frameIndexes_.swap(tempIndexes);
}

void ActionGroup::removeIndex(int index)
{
    frameIndexes_.remove(index);
}

void ActionGroup::addIndex(int index)
{
    frameIndexes_.insert(index);
}

void ActionGroup::changeIndexes(QSet<int> indexes)
{
    frameIndexes_ = indexes;
}

bool ActionGroup::contains(int frameIndex)
{
    if(frameIndex >= groupIndex() && frameIndex <= lastFrameIndex()) {
        return true;
    }
    return false;
}

bool ActionGroup::hasFrame(int frameIndex)
{
    return frameIndexes_.contains(frameIndex);
}

std::set<int> ActionGroup::frameIndexesStdSet()
{
    auto stdList = frameIndexes_.toList().toStdList();
    return std::set<int>(stdList.begin(),stdList.end());
}
