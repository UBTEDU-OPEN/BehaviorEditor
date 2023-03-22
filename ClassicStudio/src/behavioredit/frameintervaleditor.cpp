#include "frameintervaleditor.h"
#include "ui_frameintervaleditor.h"

#include <QLineEdit>
#include <QIntValidator>

FrameIntervalEditor::FrameIntervalEditor(bool bAdd,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrameIntervalEditor),
    add_(bAdd)
{
    ui->setupUi(this);
    if(bAdd)
    {
        ui->name->setText("Add Interval");
        ui->description->setText("Will add intervals of the number specified between selected frames, so that the behavior will be more slowly.");
    } else {
        ui->name->setText("Reduce Interval");
        ui->description->setText("Will reduce intervals of the number specified between selected frames, so that the behavior will be more fast.");
    }
    ui->frameCount->setValidator(new QIntValidator(1,999999,this));
    setWindowFlags(Qt::FramelessWindowHint);
}

FrameIntervalEditor::~FrameIntervalEditor()
{
    delete ui;
}

void FrameIntervalEditor::on_frameCount_textEdited(const QString &/*arg1*/)
{
    ui->ok->setEnabled(true);
}

void FrameIntervalEditor::on_cancel_clicked()
{
    close();
}

void FrameIntervalEditor::on_ok_clicked()
{
    int interval = ui->frameCount->text().toInt();
    if(interval>0 && interval < 999999)
    {
        emit sigIntervalChange(add_,interval);
    }
    close();
}
