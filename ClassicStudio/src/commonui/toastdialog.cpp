#include "toastdialog.h"
#include "ui_toastdialog.h"
#include<QTimer>

ToastDialog::ToastDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ToastDialog)
{
    ui->setupUi(this);
    setModal(true);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    QTimer::singleShot(2000,this,[this]{
        close();
    });
}

ToastDialog::~ToastDialog()
{
    delete ui;
}

void ToastDialog::setDisplayText(QString text)
{
    ui->label->setText(text);
    adjustSize();
}
