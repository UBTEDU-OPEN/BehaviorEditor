#include "distancewidget.h"
#include "ui_distancewidget.h"

DistanceWidget::DistanceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DistanceWidget)
{
    ui->setupUi(this);
}

DistanceWidget::~DistanceWidget()
{
    delete ui;
}

void DistanceWidget::setDistance(const QString& distance)
{
    ui->distance->setText(distance);
}
