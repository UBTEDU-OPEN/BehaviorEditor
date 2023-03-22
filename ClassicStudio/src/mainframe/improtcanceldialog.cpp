#include "improtcanceldialog.h"
#include "ui_improtcanceldialog.h"

ImprotCancelDialog::ImprotCancelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImprotCancelDialog)
{
    ui->setupUi(this);
}

ImprotCancelDialog::~ImprotCancelDialog()
{
    delete ui;
}

void ImprotCancelDialog::on_closeBtn_clicked()
{
    this->close();
}


void ImprotCancelDialog::on_noBtn_clicked()
{
    this->close();
}

void ImprotCancelDialog::on_yesBtn_clicked()
{
    emit cancelImprot();
    this->close();

}
