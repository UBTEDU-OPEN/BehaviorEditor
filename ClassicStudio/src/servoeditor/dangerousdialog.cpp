#include "dangerousdialog.h"
#include "ui_dangerousdialog.h"
#include"classstudiosettings.h"


DangerousDialog::DangerousDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DangerousDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(true);
}

DangerousDialog::~DangerousDialog()
{
    delete ui;
}

void DangerousDialog::on_closeBtn_clicked()
{
    this->close();
}

void DangerousDialog::on_iKnowBtn_clicked()
{
    emit sigSure();
    this->close();

}

void DangerousDialog::on_noMoreTipsBtn_clicked()
{
    ClassStudioSettings settings;
    settings.setNoMoreTips(true);
    emit sigSure();
    this->close();
}
