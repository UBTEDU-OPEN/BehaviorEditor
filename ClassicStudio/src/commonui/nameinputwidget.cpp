#include "nameinputwidget.h"
#include "ui_nameinputwidget.h"

#include <QRegExp>
#include <QValidator>

NameInputWidget::NameInputWidget(const QString& label, const QString& originName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NameInputWidget)
{
    ui->setupUi(this);
    ui->label->setText(label);
    ui->lineEdit->setText(originName);
    ui->warning->setVisible(false);
    QRegExp regx("^[a-zA-Z0-9_\u4e00-\u9fa5]+$");
    QValidator *validator = new QRegExpValidator(regx,ui->lineEdit);
    ui->lineEdit->setValidator(validator);
    connect(ui->lineEdit, &QLineEdit::textChanged, [this](QString s){
        s.replace(QString::fromLocal8Bit("【"), "");
        s.replace(QString::fromLocal8Bit("】"), "");
        s.replace(QString::fromLocal8Bit("？"), "");
        s.replace(QString::fromLocal8Bit("！"), "");
        s.replace(QString::fromLocal8Bit("·"), "");
        s.replace(QString::fromLocal8Bit("￥"), "");
        s.replace(QString::fromLocal8Bit("……"), "");
        s.replace(QString::fromLocal8Bit("（"), "");
        s.replace(QString::fromLocal8Bit("）"), "");
        s.replace(QString::fromLocal8Bit("——"), "");
        s.replace(QString::fromLocal8Bit("、"), "");
        s.replace(QString::fromLocal8Bit("："), "");
        s.replace(QString::fromLocal8Bit("；"), "");
        s.replace(QString::fromLocal8Bit("“"), "");
        s.replace(QString::fromLocal8Bit("”"), "");
        s.replace(QString::fromLocal8Bit("’"), "");
        s.replace(QString::fromLocal8Bit("‘"), "");
        s.replace(QString::fromLocal8Bit("《"), "");
        s.replace(QString::fromLocal8Bit("》"), "");
        s.replace(QString::fromLocal8Bit("，"), "");
        s.replace(QString::fromLocal8Bit("。"), "");
        ui->lineEdit->setText(s);
        if(s.isEmpty()) {
            emit nameIsEmpty(true);
        } else {
            emit nameIsEmpty(false);
        }
    });
    ui->lineEdit->setMaxLength(16);
}

NameInputWidget::~NameInputWidget()
{
    delete ui;
}

QString NameInputWidget::newName()
{
    return ui->lineEdit->text();
}

void NameInputWidget::setWarningText(QString warningText)
{
    ui->warning->setText(warningText);
    ui->warning->setVisible(true);
}
