#include "savefiledialog.h"
#include "ui_savefiledialog.h"
#include<QFile>
#include<QFileDialog>
#include<QString>
#include <QRegExp>
#include <QValidator>
#include<QDebug>

SaveFileDialog::SaveFileDialog(QString behaviorPath,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveFileDialog),
    mBehaviorPath(behaviorPath)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(true);
    QFile styleSheet(":/image/res/qss/savefiledialog.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        setStyleSheet(styleSheet.readAll());
    }
//    ui->filePathEdit->setText(qApp->applicationDirPath());
    if(mBehaviorPath.isEmpty()){
        ui->filePathEdit->setText(qApp->applicationDirPath()+"/temp");
    }else {
        QFileInfo fileInfo(mBehaviorPath);
        if(fileInfo.baseName().compare("ubt_untitled") != 0) {
            ui->fileNameEdit->setText(fileInfo.baseName());
        } else {
            ui->fileNameEdit->setText("");
        }
        ui->filePathEdit->setText(fileInfo.absolutePath());
    }
    ui->okBtn->setEnabled(false);
    ui->okBtn->setFocus();
    QRegExp regx("^[a-zA-Z0-9_\u4e00-\u9fa5]+$");
    QValidator *validator = new QRegExpValidator(regx,ui->fileNameEdit);
    ui->fileNameEdit->setValidator(validator);
    connect(ui->fileNameEdit, &QLineEdit::textChanged, [this](QString s){
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
        ui->fileNameEdit->setText(s);
        if(!s.isEmpty()) {
            ui->okBtn->setEnabled(true);
        } else {
            ui->okBtn->setEnabled(false);
        }
    });
    ui->fileNameEdit->setMaxLength(16);

    connect(ui->filePathEdit,&QLineEdit::textChanged,[this](const QString& s){
        if(s.isEmpty()) {
            ui->okBtn->setEnabled(false);
        } else {
            ui->okBtn->setEnabled(true);
        }
    });
}

SaveFileDialog::~SaveFileDialog()
{
    delete ui;
}

void SaveFileDialog::on_closeBtn_clicked()
{
    this->close();
}


void SaveFileDialog::on_choosePathBtn_clicked()
{
    QString fileDir = ui->filePathEdit->text().trimmed();
    if(fileDir.isEmpty()){
        fileDir = qApp->applicationDirPath();
    }else {
        bool exist = QFile::exists(fileDir);
        if(!exist){
            fileDir = qApp->applicationDirPath();
        }
    }
    QString path =  QFileDialog::getExistingDirectory(
        nullptr,
        tr("Save"),
        fileDir,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!path.isEmpty()){
        ui->filePathEdit->setText(path);
    }
    qDebug() << "path===" << path;
}

void SaveFileDialog::on_cancelBtn_clicked()
{
    this->close();
}

void SaveFileDialog::on_okBtn_clicked()
{
    QString fileName = ui->fileNameEdit->text().trimmed();
    QString fileDir = ui->filePathEdit->text().trimmed();
    bool exist = QFile::exists(fileDir);
    if(!fileName.isEmpty() && !fileDir.isEmpty()){
        if(exist){
            emit sigSaveFile(fileName,fileDir);
            this->close();
        }else {
            qDebug() << "file Dir is not exist";
        }
    }else {
        qDebug() << "filename isEmpty or file Dir isEmpty";
    }

}
