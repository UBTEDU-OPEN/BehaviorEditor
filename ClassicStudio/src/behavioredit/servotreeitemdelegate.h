#ifndef SERVOTREEITEMDELEGATE_H
#define SERVOTREEITEMDELEGATE_H

#include <QStyledItemDelegate>

class ServoTreeItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ServoTreeItemDelegate(QObject *parent = nullptr);
    ~ServoTreeItemDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // SERVOTREEITEMDELEGATE_H
