#include "servoitem.h"

#include <QPainter>
#include <QPaintEvent>

ServoItem::ServoItem(QWidget *parent)
    : ServoItem(0, -1, -1, 0, parent)
{
}

ServoItem::ServoItem(int frameIdx, int layerIdx, int srvId, double srvValue, QWidget *parent)
    : QWidget(parent)
    , parentNode_(nullptr)
    , selectedState_(eUnSelected)
    , frameIndex_(frameIdx)
    , layerIndex_(layerIdx)
    , servoId_(srvId)
    , servoAngle_(srvValue)
{
    childNodes_.clear();

    setFixedSize(kServoItemWidth, kServoItemHeight);
    setVisible(false);
    setMouseTracking(true);
}

ServoItem::~ServoItem()
{
}

QSet<ServoItem*> ServoItem::getAncestorNodes() const
{
    QSet<ServoItem *> ancestors;
    auto p = getParentNode();
    while (p) {
        ancestors.insert(p);
        p = p->getParentNode();
    }
    return ancestors;
}

QSet<ServoItem *> ServoItem::getDescendantNodes() const
{
    QSet<ServoItem *> descendants;
    for (auto childNode : childNodes_) {
        descendants.insert(childNode);
        descendants.unite(childNode->getDescendantNodes());
    }
    return descendants;
}

ServoItem *ServoItem::getDescendantNode(int layerIndex) const
{
    auto descendants = getDescendantNodes();
    for (auto descendant : descendants) {
        if (layerIndex == descendant->layerIndex_) {
            return descendant;
        }
    }
    return nullptr;
}

void ServoItem::deleteDescendantNodes()
{
    auto descendants = getDescendantNodes();
    for (auto descendant : descendants) {
        descendant->setVisible(false);
        descendant->deleteLater();
    }
}

bool ServoItem::hasChildNode(int layerIndex) const
{
    for (auto itr = childNodes_.cbegin(); itr != childNodes_.cend(); ++itr) {
        if ((*itr)->layerIndex_ == layerIndex) {
            return true;
        }
    }
    return false;
}

ServoItem *ServoItem::getChild(int layerIndex) const
{
    for (auto itr = childNodes_.cbegin(); itr != childNodes_.cend(); ++itr) {
        if ((*itr)->layerIndex_ == layerIndex) {
            return *itr;
        }
    }
    return nullptr;
}

ServoItem *ServoItem::removeChildNode(ServoItem *child)
{
    if (childNodes_.remove(child)) {
        return child;
    } else {
        return nullptr;
    }
}

ServoItem *ServoItem::removeChildNode(int layerIndex)
{
    for (auto child : childNodes_) {
        if (child->layerIndex_ == layerIndex) {
            return removeChildNode(child);
        }
    }
    return nullptr;
}

bool ServoItem::hasSibling() const
{
    if (!parentNode_) {
        return (parentNode_->childNodes_.count() > 1);
    }
    return false;
}

void ServoItem::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}

void ServoItem::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.save();
    switch (selectedState_) {
    case eUnSelected: {
        painter.fillRect(e->rect(), QBrush(Qt::lightGray));
    }
        break;
    case eSelected: {
        painter.fillRect(e->rect(), QBrush(Qt::blue));
    }
        break;
    case ePartiallySelected: {
        painter.fillRect(e->rect(), QBrush(Qt::green));
    }
        break;
    default:
        break;
    }
    painter.restore();
}
