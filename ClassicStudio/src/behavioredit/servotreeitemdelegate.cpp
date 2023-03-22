#include "servotreeitemdelegate.h"

#include <QStyleOptionViewItem>
#include <QModelIndex>

ServoTreeItemDelegate::ServoTreeItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

ServoTreeItemDelegate::~ServoTreeItemDelegate()
{
}

void ServoTreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}
