#include "connectedwidget.h"
#include "ui_connectedwidget.h"

ConnectedWidget::ConnectedWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectedWidget)
{
    ui->setupUi(this);
}

ConnectedWidget::~ConnectedWidget()
{
    delete ui;
}

void ConnectedWidget::setSn(QString sn)
{
    ui->snLabel->setText(sn);
}

void ConnectedWidget::on_disconnectBtn_clicked()
{
    emit disconnectBtnClicked();
}
