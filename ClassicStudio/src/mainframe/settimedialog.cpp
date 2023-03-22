#include "settimedialog.h"
#include "ui_settimedialog.h"
#include<QDebug>

SetTimeDialog::SetTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetTimeDialog)
{
    ui->setupUi(this);
    connect(ui->setTimeBox,SIGNAL(valueChanged(int)),this,SIGNAL(valueChange(int)));
}

SetTimeDialog::~SetTimeDialog()
{
    delete ui;
}

void SetTimeDialog::closeEvent(QCloseEvent *)
{
    emit closed();
}
