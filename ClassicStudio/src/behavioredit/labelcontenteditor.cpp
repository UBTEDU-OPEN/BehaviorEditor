#include "labelcontenteditor.h"
#include "ui_labelcontenteditor.h"

#include <QDebug>
#include <QTime>
#include <QLineEdit>

LabelContentEditor::LabelContentEditor(const int frameIndex, const int fps, const QString content,QWidget *parent)
    : QDialog(parent),
    ui(new Ui::LabelContentEditor),
    frameIndex_(frameIndex),
    fps_(fps)
{
    ui->setupUi(this);

    ui->warning->hide();
    ui->frame->setText(QString::number(frameIndex));
    ui->timeStamp->setText(calcTimeStamp(frameIndex));
    ui->timeStamp->setDisabled(true);
    ui->ok->setDisabled(true);
    ui->content->setText(content);
    ui->content->setFocus();

    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);

    connect(ui->content,&QTextEdit::textChanged,this,&LabelContentEditor::onTextChanged);
    connect(ui->frame,&QLineEdit::textChanged,this,&LabelContentEditor::onFrameChanged);
//    setStyleSheet("background-color:white");
}

LabelContentEditor::~LabelContentEditor()
{
    delete ui;
}

void LabelContentEditor::on_ok_clicked()
{
    QString text = ui->content->toPlainText();
    int frameIndex = ui->frame->text().toInt();
    if(frameIndex == 0) //calc failed,revert
    {
        frameIndex = frameIndex_;
    }
    emit sigContentChanged(frameIndex,text);
    close();
}

void LabelContentEditor::on_cancel_clicked()
{
    close();
}

void LabelContentEditor::onTextChanged()
{
    QString text = ui->content->toPlainText();
    if(text.length()>200)
    {
        ui->warning->show();
        ui->ok->setDisabled(true);
    }
    else
    {
        ui->warning->hide();
        ui->ok->setDisabled(false);
    }
}

QString LabelContentEditor::calcTimeStamp(int frameIndex)
{
    int ms = frameIndex*1000/fps_;
    return QTime::fromMSecsSinceStartOfDay(ms).toString("hh:mm:ss.zzz");
}

void LabelContentEditor::onFrameChanged()
{
    int frameIndex = ui->frame->text().toInt();
    if(frameIndex != 0)
    {
        ui->timeStamp->setText(calcTimeStamp(frameIndex));
    }
    ui->ok->setDisabled(false);

}
