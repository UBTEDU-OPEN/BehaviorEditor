#ifndef ROBOTACTIONITEM_H
#define ROBOTACTIONITEM_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>

class RobotActionItem : public QWidget
{
    Q_OBJECT
public:
    explicit RobotActionItem(QString actionName, QWidget *parent = nullptr);
    QString getName() { return actionName_; }

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void itemSelected(const QString&);
    void itemTriggered(const QString&);

private:
    QString actionName_;
};

#endif // ROBOTACTIONITEM_H
