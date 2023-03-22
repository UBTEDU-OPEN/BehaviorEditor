#include "volumeslider.h"
#include "ui_volumeslider.h"

#include <QStyleOption>
#include <QStyle>
#include <QPaintEvent>
#include <QPainter>
#include <QCloseEvent>
#include <QDebug>
#include <QApplication>

VolumeSlider::VolumeSlider(int volume, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VolumeSlider),
    oldValue_(volume)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->volumeSlider->setValue(volume);
    connect(ui->volumeSlider,&QSlider::sliderReleased,[this](){
        int value = ui->volumeSlider->value();
        if(value != oldValue_) {
            oldValue_ = value;
            emit volumeChanged(value);
        }
    });
}

VolumeSlider::~VolumeSlider()
{
    delete ui;
}

void VolumeSlider::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
    QWidget::paintEvent(event);
}

void VolumeSlider::closeEvent(QCloseEvent* event)
{
    qDebug() << "VolumeSlider::closeEvent";
    QWidget::closeEvent(event);
}
