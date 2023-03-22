#include "horizontalheaderview.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

HorizontalHeaderView::HorizontalHeaderView(Qt::Orientation orientation, QWidget *parent)
    : QHeaderView(orientation,parent)
{
    m_selectAll = new QCheckBox(this);
//    m_selectAll->setText(tr("Index"));
    m_selectAll->setVisible(true);
    connect(m_selectAll,&QCheckBox::clicked,this,&HorizontalHeaderView::sigSelectAll);
}

void HorizontalHeaderView::updateGeometries()
{
    m_selectAll->setGeometry(sectionPosition(0),0,sectionSize(0),height());
    QHeaderView::updateGeometries();
}

void HorizontalHeaderView::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton){
        emit sigLButtonPressEvent(logicalIndexAt(event->pos()));
    }
    QHeaderView::mousePressEvent(event);
}

void HorizontalHeaderView::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons().testFlag(Qt::LeftButton)){
        emit sigLButtonMoveEvent(logicalIndexAt(event->pos()));
    }
    QHeaderView::mouseMoveEvent(event);
}

void HorizontalHeaderView::onSelectChanged(int state)
{
    if(state == 0){
        m_selectAll->setCheckState(Qt::Unchecked);
    } else if(state == 1){
        m_selectAll->setCheckState(Qt::PartiallyChecked);
    } else if(state == 2){
        m_selectAll->setCheckState(Qt::Checked);
    }
}
