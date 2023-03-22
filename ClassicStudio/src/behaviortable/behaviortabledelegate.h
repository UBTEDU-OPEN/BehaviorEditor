#ifndef BEHAVIORTABLEDELEGATE_H
#define BEHAVIORTABLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QTableView>
#include <QPixmap>

class BehaviorTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit BehaviorTableDelegate(QObject* parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    void setTableViewPtr(QTableView* tableView){m_pTableView = tableView;}

private:
    QTableView* m_pTableView;
    QPixmap m_checkedPixmap;
    QPixmap m_partiallyCheckedPixmap;
    QPixmap m_uncheckedPixmap;
};

#endif // BEHAVIORTABLEDELEGATE_H
