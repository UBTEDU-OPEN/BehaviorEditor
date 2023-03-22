#ifndef DISTANCEWIDGET_H
#define DISTANCEWIDGET_H

#include <QWidget>

namespace Ui {
class DistanceWidget;
}

class DistanceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DistanceWidget(QWidget *parent = nullptr);
    ~DistanceWidget();
    void setDistance(const QString& distance);

private:
    Ui::DistanceWidget *ui;
};

#endif // DISTANCEWIDGET_H
