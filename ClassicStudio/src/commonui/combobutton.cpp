#include "combobutton.h"

ComboButton::ComboButton(QWidget *parent)
    : QWidget(parent)
    , promptMenu_(nullptr)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    functionButton_ = new QPushButton(this);
    functionButton_->setObjectName("functionButton");

    menuButton_ = new QPushButton(this);
    menuButton_->setObjectName("menuButton");
    menuButton_->setIcon(QIcon(":/image/res/image/button_icon_down.png"));

    separatorLabel_ = new QLabel(this);
    separatorLabel_->setFixedSize(1,24);
    separatorLabel_->setPixmap(QPixmap(":/image/res/image/line.png"));
    separatorLabel_->setStyleSheet("background-color:#5B6270;");

    layout->addWidget(functionButton_);
    layout->addWidget(separatorLabel_);
    layout->addWidget(menuButton_);

    connect(functionButton_,&QPushButton::clicked,this,&ComboButton::sigFunctionButtonClicked);
    connect(menuButton_,&QPushButton::clicked,this,&ComboButton::showPromptMenu);
}

void ComboButton::setPromptMenu(QMenu* menu)
{
    promptMenu_ = menu;
    promptMenu_->setWindowFlags(promptMenu_->windowFlags() | Qt::NoDropShadowWindowHint);
    promptMenu_->setAttribute(Qt::WA_TranslucentBackground);
}

void ComboButton::setFunctionButtonText(const QString& text)
{
    functionButton_->setText(text);
}

void ComboButton::showPromptMenu()
{
    if(promptMenu_ != nullptr) {

        // 调整按钮样式
        menuButton_->setIcon(QIcon(":/image/res/image/button_icon_up_highlight.png"));

        // 显示菜单
        QPoint pTL = mapToGlobal(functionButton_->geometry().topLeft()); // 左侧功能按钮左上角的坐标
        QPoint pBR = mapToGlobal(menuButton_->geometry().bottomRight()); // 左侧功能按钮左上角的坐标
        promptMenu_->move(QPoint(pTL.x(), pTL.y() - promptMenu_->sizeHint().height())); // 设置菜单位置
        if(promptMenu_->width() < width()) {
            promptMenu_->setFixedSize(QSize(pBR.x() - pTL.x(),promptMenu_->sizeHint().height())); // 设置菜单尺寸
        }
        promptMenu_->show();

    }
}

void ComboButton::hideSeparator()
{
    separatorLabel_->hide();
}

void ComboButton::setEnabled(bool enabled)
{

    if(enabled == false) menuButton_->setIcon(QIcon(":/image/res/image/button_icon_down.png"));
    QWidget::setEnabled(enabled);
}
