#include "behaviorcurvewidget.h"


void BehaviorCurveWidget::initMenu(QWidget* parent)
{
    m_curveMenu = new QMenu(parent);
    m_lineCurveAction = new QAction(tr("Line"),m_curveMenu);
    m_curveMenu->addAction(m_lineCurveAction);
    m_curveMenu->addSeparator();
    m_bezierCurveAction = new QAction(tr("Bezier"),m_curveMenu);
    m_curveMenu->addAction(m_bezierCurveAction);
}

BehaviorCurveWidget::BehaviorCurveWidget(QWidget* parent)
    : QPushButton(parent)
{
    initMenu(this);
    setMenu(m_curveMenu);

    connect(this,&BehaviorCurveWidget::clicked,[this]{showMenu();});
    connect(m_lineCurveAction,&QAction::triggered,[this]{setText(m_lineCurveAction->text());});
    connect(m_bezierCurveAction,&QAction::triggered,[this]{setText(m_bezierCurveAction->text());});
}
