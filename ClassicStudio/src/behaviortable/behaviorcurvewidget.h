#ifndef BEHAVIORCURVEWIDGET_H
#define BEHAVIORCURVEWIDGET_H

#include <QPushButton>
#include <QMenu>
#include <QAction>

class BehaviorCurveWidget : public QPushButton
{
    Q_OBJECT
public:
    explicit BehaviorCurveWidget(QWidget* parent = nullptr);
    void initMenu(QWidget* parent);

private:
    QMenu* m_curveMenu;
    QAction* m_lineCurveAction;
    QAction* m_bezierCurveAction;
};

#endif // BEHAVIORCURVEWIDGET_H
