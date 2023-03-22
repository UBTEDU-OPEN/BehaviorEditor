#include "inputsnwidget.h"
#include "ui_inputsnwidget.h"
#include<QRegExpValidator>
#include<QDebug>


InputSnWidget::InputSnWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputSnWidget)
{
    ui->setupUi(this);
    ui->connectBtn->setEnabled(false);
    ClassStudioSettings settings;
    QString sn = settings.getConnectedRobotSN();
    if(!sn.isEmpty()){
        int size = sn.size();
        QString ssn = sn.remove(0,size-4);
        for(int i=0;i<ssn.size();i++){
            QChar c = ssn.at(i);
            if(i == 0){
                ui->snEdit1->setText(c);
            }else if(i == 1){
                ui->snEdit2->setText(c);
            }else if(i == 2){
                ui->snEdit3->setText(c);
            }else {
                ui->snEdit4->setText(c);
            }
            ui->connectBtn->setEnabled(true);
        }
    }
    connect(ui->snEdit1,&QLineEdit::textChanged,this,&InputSnWidget::onTextChange);
    connect(ui->snEdit2,&QLineEdit::textChanged,this,&InputSnWidget::onTextChange);
    connect(ui->snEdit3,&QLineEdit::textChanged,this,&InputSnWidget::onTextChange);
    connect(ui->snEdit4,&QLineEdit::textChanged,this,&InputSnWidget::onTextChange);

    ui->connErrorLabel->setVisible(false);
    QRegExpValidator *exp = new QRegExpValidator(QRegExp("[0-9A-Za-z]+$"),this);
    ui->snEdit1->setValidator(exp);
    ui->snEdit2->setValidator(exp);
    ui->snEdit3->setValidator(exp);
    ui->snEdit4->setValidator(exp);
    ui->snEdit1->setFocus();
    ui->snEdit2->installEventFilter(this);
    ui->snEdit3->installEventFilter(this);
    ui->snEdit4->installEventFilter(this);
}

InputSnWidget::~InputSnWidget()
{
    delete ui;
}



void InputSnWidget::onTextChange()
{
    QLineEdit * edit = dynamic_cast<QLineEdit *>(sender());
    QString objectName = edit->objectName();
    if(objectName == "snEdit1"){
        if(!ui->snEdit1->text().isEmpty()){
            ui->snEdit2->setFocus();
        }
    }else if(objectName == "snEdit2"){
        if(!ui->snEdit2->text().isEmpty()){
            ui->snEdit3->setFocus();
        }
    }else if(objectName == "snEdit3"){
        if(!ui->snEdit3->text().isEmpty()){
            ui->snEdit4->setFocus();
        }
    }
    if(!ui->snEdit1->text().isEmpty() &&!ui->snEdit2->text().isEmpty()
            &&!ui->snEdit3->text().isEmpty() &&!ui->snEdit4->text().isEmpty()){
        ui->connectBtn->setEnabled(true);
    }else {
        ui->connectBtn->setEnabled(false);
    }
}

void InputSnWidget::on_connectBtn_clicked()
{
    QString sn1 = ui->snEdit1->text();
    QString sn2 = ui->snEdit2->text();
    QString sn3 = ui->snEdit3->text();
    QString sn4 = ui->snEdit4->text();
    QString sn = sn1 + sn2 + sn3 + sn4;
    emit connectBtnClicked(sn);
}

void InputSnWidget::setErroeView(int errorCode)
{
    ui->connErrorLabel->setVisible(true);

}

bool InputSnWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Backspace){
            if(obj == ui->snEdit2){
                QString sn2 = ui->snEdit2->text();
                if(sn2.isEmpty()){
                    ui->snEdit1->setFocus();
                }
            }else if(obj == ui->snEdit3){
                QString sn3 = ui->snEdit3->text();
                if(sn3.isEmpty()){
                    ui->snEdit2->setFocus();
                }
            }else if(obj == ui->snEdit4){
                QString sn4 = ui->snEdit4->text();
                if(sn4.isEmpty()){
                    ui->snEdit3->setFocus();
                }
            }
        }
        qDebug("Ate key press %d", keyEvent->key());
        return QObject::eventFilter(obj, event);;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}


