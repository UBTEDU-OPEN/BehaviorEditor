#ifndef ROBOTACTIONLISTITEM_H
#define ROBOTACTIONLISTITEM_H

#include <QWidget>
#include<QString>
#include<QListWidgetItem>

namespace Ui {
class RobotActionListItem;
}

class RobotActionListItem : public QWidget
{
    Q_OBJECT

public:
    explicit RobotActionListItem(QWidget *parent = nullptr);
    ~RobotActionListItem();
    void setActionName(QString actionName);
    QString getActionName();
    void setSelcetStyle(bool isSelect);
    QListWidgetItem *getItem();
    void setItem(QListWidgetItem * item);
protected:
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void itemSelected(const QString&);
    void itemTriggered(const QString&);
private:
    Ui::RobotActionListItem *ui;
    QString actionName_;
    QListWidgetItem *mItem;
};

#endif // ROBOTACTIONLISTITEM_H
