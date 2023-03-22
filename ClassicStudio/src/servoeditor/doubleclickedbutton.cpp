#include "doubleclickedbutton.h"

DoubleClickedButton::DoubleClickedButton(QWidget *parent)
    : QPushButton(parent)
{

}

DoubleClickedButton::DoubleClickedButton(const QString &text, QWidget *parent)
    : QPushButton(text,parent)
{

}
DoubleClickedButton::DoubleClickedButton(const QIcon& icon, const QString &text, QWidget *parent)
    : QPushButton(icon,text,parent)
{

}

DoubleClickedButton::~DoubleClickedButton()
{

}

void DoubleClickedButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked();
    QPushButton::mouseDoubleClickEvent(event);
}
