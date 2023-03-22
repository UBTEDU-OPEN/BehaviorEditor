#include "templatebutton.h"

const QString TemplateButton::arrBackgroundResources[6] = { ":/TemplateButtonBackground/res/temp_btn_bg_purple.png",
							    ":/TemplateButtonBackground/res/temp_btn_bg_blue.png",
							    ":/TemplateButtonBackground/res/temp_btn_bg_cyan.png",
							    ":/TemplateButtonBackground/res/temp_btn_bg_green.png",
							    ":/TemplateButtonBackground/res/temp_btn_bg_yellow.png",
							    ":/TemplateButtonBackground/res/temp_btn_bg_red.png" };

TemplateButton::TemplateButton(int number, QString name, const QIcon &qiNormalIcon, const QIcon &qiHoveringIcon, const QIcon &qiSelectedIcon, QWidget *parent):
	QWidget(parent), name(name), number(number){
	// 子控件
	this->qlButtonIcon = new QLabel(this);
	this->qlButtonText = new QLabel(this);
	this->qvblVertialLayout = new QVBoxLayout(this);
	this->qhblHorizontalLayout = new QHBoxLayout(this);
        
	// 设置上方图标的尺寸
	this->qpNormalIcon = qiNormalIcon.pixmap(QSize(20, 20));
	this->qpHoveringIcon = qiHoveringIcon.pixmap(QSize(20, 20));
	this->qpSelectedIcon = qiSelectedIcon.pixmap(QSize(20, 20));
        
	// 上方的图标
	this->qlButtonIcon->setObjectName("ButtonIcon");
	this->qlButtonIcon->setFixedSize(QSize(20, 20));
	this->qlButtonText->setAlignment(Qt::AlignCenter);
	this->qlButtonIcon->setPixmap(this->qpNormalIcon);
	this->qlButtonIcon->setStyleSheet("background-image: none; background-color: #373C49;");
        
	// 下方的说明文字
	this->qlButtonText->setObjectName("ButtonText");
	this->qlButtonText->setText(name);
	this->qlButtonText->setAlignment(Qt::AlignCenter);
	this->qlButtonText->setStyleSheet(
		"color: rgba(127,127,127,1); background-image: none;"
		"background-color: #373C49;"
		"width: 100%;"
		"font-family:'Microsoft Yahei';"
		"font-size: 12px;"
		"line-height: 16px;");
        
	// 将控件加入布局
	this->qvblVertialLayout->addWidget(this->qlButtonIcon, 0, Qt::AlignCenter);
	this->qvblVertialLayout->addWidget(this->qlButtonText, 0, Qt::AlignHCenter | Qt::AlignTop);
	setLayout(qvblVertialLayout); // 应用布局
        
	this->setFixedSize(QSize(60, 56));
	this->setCursor(Qt::PointingHandCursor);
}

TemplateButton::~TemplateButton(){
	delete qlButtonIcon;
	delete qlButtonText;
	delete qhblHorizontalLayout;
	delete qvblVertialLayout;
}

void TemplateButton::unselect(){
	this->isSelected = false;
	this->qlButtonIcon->setPixmap(this->qpNormalIcon);
	this->qlButtonText->setStyleSheet("color: rgba(127,127,127,1)");
}

int TemplateButton::getNumber() const {
	return this->number;
}

QString TemplateButton::getName() const {
	return this->name;
}

void TemplateButton::mousePressEvent(QMouseEvent *ev){
	this->qpMouseClickedPosition = QPoint(ev->x(), ev->y());
}

void TemplateButton::mouseReleaseEvent(QMouseEvent *ev){
	if(qpMouseClickedPosition == QPoint(ev->x(), ev->y())){
		// 判断状态
		isSelected = !isSelected;
		if(isSelected){
			this->qlButtonIcon->setPixmap(this->qpSelectedIcon);
			this->qlButtonText->setStyleSheet("color: #5DFCFF");
		}else{
			this->qlButtonIcon->setPixmap(this->qpNormalIcon);
			this->qlButtonText->setStyleSheet("color: rgba(127,127,127,1)");
		}
		emit onClicked(*this);
	}
}

void TemplateButton::enterEvent(QEvent*){
	if(this->isSelected == false){
		this->qlButtonIcon->setPixmap(this->qpHoveringIcon);
		this->qlButtonText->setStyleSheet("color: #FFFFFF");
	}
}

void TemplateButton::leaveEvent(QEvent*){
	if(this->isSelected == false){
		this->qlButtonIcon->setPixmap(this->qpNormalIcon);
		this->qlButtonText->setStyleSheet("color: rgba(127,127,127,1)");
	}
}

void TemplateButton::paintEvent(QPaintEvent *ev){
	QPainter painter(this);
	painter.drawPixmap(rect(), QPixmap(TemplateButton::arrBackgroundResources[this->number % 6]), QRect());
}