#include "axis.h"

#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include <QDebug>

Axis::Axis(Axis::AxisPostion axisposition, double minvalue, double maxvalue, double splitvalue, double tickinterval, double tickdistance, QString labelformat, QWidget *parent)
    : QWidget(parent)
    , axisPosition_(axisposition)
    , minValue_(minvalue)
    , maxValue_(maxvalue)
    , splitValue_(splitvalue)
    , tickInterval_(tickinterval)
    , tickDistanceBase_(tickdistance)
    , scaleFactor_(1)
    , labelFormat_(labelformat)
{
    setFixedWidth(calculateProperWidth());
}

Axis::~Axis()
{
}

void Axis::setAxisPostion(Axis::AxisPostion pos)
{
    axisPosition_ = pos;
    updateElements();
    update();
}

void Axis::setMinValue(double min)
{
    minValue_ = min;
    setFixedWidth(calculateProperWidth());
    updateElements();
    update();
}

void Axis::setMaxValue(double max)
{
    maxValue_ = max;
    setFixedWidth(calculateProperWidth());
    updateElements();
    update();
}

void Axis::setSplitValue(double split)
{
    splitValue_ = split;
    updateElements();
    update();
}

void Axis::setTickInterval(double interval)
{
    tickInterval_ = interval;
    setFixedWidth(calculateProperWidth());
    updateElements();
    update();
}

void Axis::setTickDistanceBase(double distance)
{
    tickDistanceBase_ = distance;
    setFixedWidth(calculateProperWidth());
    updateElements();
    update();
}

void Axis::setLabelFormat(const QString &format)
{
    labelFormat_ = format;
    updateElements();
    update();
}

void Axis::setScaleFactor(double scale)
{    
    scaleFactor_ = scale;
    setFixedWidth(calculateProperWidth());
    updateElements();    
    update();
}

void Axis::changeTick(double interval, double distance)
{
    tickInterval_ = interval;
    tickDistanceBase_ = distance / scaleFactor_;
    maxValue_ = width() * tickInterval_ / tickDistance();
    updateElements();
    update();
}

void Axis::paintEvent(QPaintEvent *e)
{
//    qDebug() << "Axis::paintEvent rect:" << e->rect() << "region:" << e->region();
    double tickDistance_scaleFactor = tickDistance();
    int start = qFloor(e->rect().x() / tickDistance_scaleFactor);
    int end = qCeil((e->rect().x() + e->rect().width()) / tickDistance_scaleFactor);
    end = qMin(end, scaleTicks_.count());

    QPainter painter(this);
//    QColor tickColor(215, 215, 215);
    QColor tickColor(255, 255, 255,127);
    if (axisPosition_ == Axis::Top) {
        painter.save();
        painter.fillRect(axisBgRect_, QColor(37, 42, 53));
        painter.fillRect(validIntervalRect_, QColor(37, 42, 53));
        painter.fillRect(invalidIntervalRect_, QColor(37, 42, 53));
        painter.setPen(tickColor);
        for (int i = start; i < end; ++i) {
            auto line = scaleTicks_.at(i);
            if(i % 5 != 0) {
                auto pos1 = line.p1();
                pos1.setY(pos1.y()*6/5);
                line.setP1(pos1);
            }
            painter.drawLine(line);
            auto label = labelFormat_.arg(labelValues_.at(i));
            if(i % 5 == 0) {
                QFontMetrics fm = painter.fontMetrics();
                int labelWidth = fm.width(label);
                auto textRect = scaleTickLabelRects_.at(i);
                textRect.setX(textRect.x()-labelWidth/2);
                textRect.setY(textRect.y() - 3);
                painter.drawText(textRect, label);
            }
        }
        painter.restore();
    } else {
        painter.save();
        painter.fillRect(axisBgRect_, QColor(37, 42, 53));
        painter.fillRect(validIntervalRect_, QColor(37, 42, 53));
        painter.fillRect(invalidIntervalRect_, QColor(37, 42, 53));
        painter.setPen(tickColor);
        for (int i = start; i < end; ++i) {
            painter.drawLine(scaleTicks_.at(i));
            auto label = labelFormat_.arg(labelValues_.at(i));
            QFontMetrics fm = painter.fontMetrics();
            int labelWidth = fm.width(label);
            auto textRect = scaleTickLabelRects_.at(i);
            textRect.setX(textRect.x()-labelWidth/2);
            textRect.setY(textRect.y() + 1);
            painter.drawText(textRect, label);
        }
        painter.restore();
    }
}

void Axis::resizeEvent(QResizeEvent *e)
{
    updateElements();
    QWidget::resizeEvent(e);
}

double Axis::calculateProperWidth() const
{
    auto properWidth = (maxValue_ - minValue_) / tickInterval_ * tickDistance();
    return properWidth;
}

void Axis::updateElements()
{
//    QRectF axisBgRect;
//    QRectF validIntervalRect;
//    QRectF invalidIntervalRect;
//    QVector<QLineF> scaleTicks;
//    QVector<QRectF> scaleTickLabelRects;

    double w = width();
    double h_2_3 = height() * 2 / 3;
    double h_1_3 = height() / 3;
    double h_2_9 = height() * 2 / 9;
    double h_4_9 = height() * 4 / 9;
    double tickDistance_scaleFactor = tickDistance();
    scaleTicks_.clear();
    scaleTickLabelRects_.clear();
    labelValues_.clear();
    if (axisPosition_ == Top) {
        axisBgRect_ = QRectF(0, 0, w, h_2_3);
        validIntervalRect_ = QRectF(0, h_2_3, w * splitValue_ / maxValue_, h_1_3);
        invalidIntervalRect_ = QRectF(w * splitValue_ / maxValue_, h_2_3, w * (maxValue_ - splitValue_) / maxValue_, h_1_3);

        double value = minValue_;
        QPointF tickStart(0, h_4_9);
        QPointF tickEnd(0, h_2_3);
        while (value <= maxValue_) {
            labelValues_.append(value);
            QLineF tickLine(tickStart, tickEnd);
            scaleTicks_.append(tickLine);
            QRectF textRect(tickStart.x(), height() * 2 / 9, tickDistance_scaleFactor, height() * 2 / 9);
            scaleTickLabelRects_.append(textRect);
            tickStart.rx() += tickDistance_scaleFactor;
            tickEnd.rx() += tickDistance_scaleFactor;
            value += tickInterval_;
        }
    } else {
        axisBgRect_ = QRectF(0, h_1_3, w , h_2_3);
        validIntervalRect_ = QRectF(0, 0, w * splitValue_ / maxValue_, h_1_3);
        invalidIntervalRect_ = QRectF(w * splitValue_ / maxValue_, 0, w * (maxValue_ - splitValue_) / maxValue_, h_1_3);

        double value = minValue_;
        QPointF tickStart(0, h_1_3);
        QPointF tickEnd(0, h_4_9);
        while (value <= maxValue_) {
            labelValues_.append(value);
            QLineF tickLine(tickStart, tickEnd);
            scaleTicks_.append(tickLine);
            QRectF textRect(tickStart.x(), h_4_9, tickDistance_scaleFactor, h_2_9);
            scaleTickLabelRects_.append(textRect);
            tickStart.rx() += tickDistance_scaleFactor;
            tickEnd.rx() += tickDistance_scaleFactor;
            value += tickInterval_;
        }
    }
}

void Axis::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        emit setCurrentFramePos(e->pos().x());
    }
}
