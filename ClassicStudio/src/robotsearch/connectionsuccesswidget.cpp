#include "connectionsuccesswidget.h"
#include "ui_connectionsuccesswidget.h"

ConnectionSuccessWidget::ConnectionSuccessWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionSuccessWidget)
{
    ui->setupUi(this);
}

ConnectionSuccessWidget::~ConnectionSuccessWidget()
{
    delete ui;
}

void ConnectionSuccessWidget::on_successBtn_clicked()
{
    emit successBtnClicked();
}
