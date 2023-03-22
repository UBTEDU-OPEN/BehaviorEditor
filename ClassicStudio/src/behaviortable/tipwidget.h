#ifndef TIPSWIDGET_H
#define TIPSWIDGET_H

#include <QWidget>

class TipWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TipWidget(QWidget *parent = nullptr);

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent* event) override;

signals:
    void sigEnter();
    void sigLeave();
};

#endif // TIPSWIDGET_H
