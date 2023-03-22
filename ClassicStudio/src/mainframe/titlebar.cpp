#include "titlebar.h"
#include "ui_titlebar.h"

#include <QPushButton>
#include <QAction>
#include <QMouseEvent>
#include <QDebug>

TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::titlebar){
	ui->setupUi(this);
	setAttribute(Qt::WA_StyledBackground);
    QFile styleSheet(":/qss/res/qss/titlebar.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        setStyleSheet(styleSheet.readAll());
    }

        
//	connect(ui->file, &QPushButton::clicked, this, &TitleBar::onFileClicked);
    connect(ui->minBtn, &QPushButton::clicked, this, &TitleBar::sigMin);
    connect(ui->maxBtn, &QPushButton::clicked, this, &TitleBar::sigMax);
    connect(ui->closeBtn, &QPushButton::clicked, this, &TitleBar::sigClose);
        
	configStyle();
}

TitleBar::~TitleBar(){
	delete ui;
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event){
	if(event->button() == Qt::LeftButton){
		emit sigMax();
	}
	QWidget::mouseDoubleClickEvent(event);
}

void TitleBar::configStyle(){
//	ui->horizontalLayout->setSpacing(0);
//	ui->horizontalLayout->setContentsMargins(0, 0, 10, 0);
//    ui->logoLabel->setFixedSize(141, 32);
//	ui->file->setFixedSize(56, 32);
//	ui->edit->setFixedSize(56, 32);
//	ui->more->setFixedSize(56, 32);
    ui->minBtn->setFixedSize(26, 26);
    ui->maxBtn->setFixedSize(26, 26);
    ui->closeBtn->setFixedSize(26, 26);
    ui->minBtn->setIconSize(QSize(10, 10));
    ui->maxBtn->setIconSize(QSize(10, 10));
    ui->closeBtn->setIconSize(QSize(10, 10));

}

void TitleBar::onFileClicked(){
//	auto pos = mapToGlobal(ui->file->geometry().bottomLeft());
//	pos.setX(pos.x() + 10);
//	pos.setY(pos.y() + 10);
//	m_mainMenu->move(pos);
//	m_mainMenu->show();
}

void TitleBar::onEdited()
{
    setEditedIconVisible(true);
}

void TitleBar::onSaved(const QString& fileName)
{
    setEditedIconVisible(false);
    setFileName(fileName);
}

void TitleBar::onOpend(const QString& fileName)
{
    setEditedIconVisible(false);
    setFileName(fileName);
}

void TitleBar::setEditedIconVisible(bool edited)
{
    ui->editedIcon->setVisible(edited);
}

void TitleBar::setFileName(const QString& fileName)
{
    ui->fileName->setText(fileName);
}
