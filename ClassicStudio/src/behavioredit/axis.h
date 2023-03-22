#ifndef AXIS_H
#define AXIS_H

#include <QWidget>
#include <QMap>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>

#include "labelblock.h"

class Axis : public QWidget
{
    Q_OBJECT

public:
    enum AxisPostion {
        Top,
        Bottom
    };

public:
    Axis(AxisPostion axisposition = Axis::Top,
         double minvalue = 0,
         double maxvalue = 1000,
         double splitvalue = 500,
         double tickinterval = 10,
         double tickdistance = 50,
         QString labelformat = "%1",
         QWidget *parent = nullptr);
    ~Axis() override;

    AxisPostion getAxisPostion() const {return axisPosition_;}
    void setAxisPostion(AxisPostion pos);
    inline double getMinValue() const {return minValue_;}
    void setMinValue(double min);
    inline double getMaxValue() const {return maxValue_;}
    void setMaxValue(double max);
    inline double getSplitValue() const {return splitValue_;}
    void setSplitValue(double split);
    inline double getTickInterval() const {return tickInterval_;}
    void setTickInterval(double interval);
    inline double getTickDistanceBase() const {return tickDistanceBase_;}
    void setTickDistanceBase(double distance);
    inline double tickDistance() const { return tickDistanceBase_ * scaleFactor_; }
    void setLabelFormat(const QString &format);
    double getScaleFactor() const {return scaleFactor_;}
    void setScaleFactor(double scale);
    void changeTick(double interval, double distance);

    double valueAtPos(double pos);
    QString valueStrAtPos(double pos);


signals:
    void setCurrentFramePos(double fi);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *e) override;
    void mousePressEvent(QMouseEvent* e) override;

private:
    double calculateProperWidth() const;
    void updateElements();

private:
    AxisPostion axisPosition_;
    double minValue_;
    double maxValue_;
    double splitValue_;
    double tickInterval_;
    double tickDistanceBase_;
    double scaleFactor_;
    QString labelFormat_;

    QRectF axisBgRect_;
    QRectF validIntervalRect_;
    QRectF invalidIntervalRect_;
    QVector<QLineF> scaleTicks_;
    QVector<QRectF> scaleTickLabelRects_;
    QVector<double> labelValues_;
};

#endif // AXIS_H
