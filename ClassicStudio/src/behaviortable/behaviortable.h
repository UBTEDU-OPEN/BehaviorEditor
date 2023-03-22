#ifndef BEHAVIORTABLE_H
#define BEHAVIORTABLE_H

#include "behaviortable_global.h"
#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QMap>
#include "widgettipdialog.h"


namespace Ui {
class BehaviorTable;
}

class BehaviorTableWidget;
class TipWidget;
class TipDialog;

class BEHAVIORTABLE_EXPORT BehaviorTable : public QWidget
{
    Q_OBJECT
public:
    explicit BehaviorTable(QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

public slots:
    void onFrameIdChanged(QMap<int, QPair<double, int>> servoInfo, bool isKeyFrame);
    void onBehaviorEditSelected(const QMap<int, QMap<int,double>> &selectedServos);
    void onClearSelection();
    void onSelectRow(int row);

private slots:
    void onAddFrameClicked();
    void on_reverse_clicked();
    void on_mirror_clicked();
    void addAll();
    void addHead();
    void addRightArm();
    void addLeftArm();
    void addRightLeg();
    void addLeftLeg();
    void mirrorLeftArmToRight();
    void mirrorRightArmToLeft();
    void mirrorLeftLegToRight();
    void mirrorRightLegToLeft();
    void reverseFull();
    void reverseHead();
    void reverseArm();
    void reverseLeg();
    void on_deleteSelection_clicked();
    void onSetCurve(int curveType);


signals:
    void sigAddFrame(int,int);
    void sigMirror(int);
    void sigReverse(int);
    void sigSelcectedChanged(QMap<int, QMap<int,double>>);

private:
    void configStyle();
    void addFrame(int bodyType);

private:
    Ui::BehaviorTable *ui;
    BehaviorTableWidget* tableWidget;
    QMenu* m_pAddBodyServoMenu;
    QAction* m_pAddAllAction;
    QAction* m_pAddRightArmAction;
    QAction* m_pAddLeftArmAction;
    QAction* m_pAddRightLegAction;
    QAction* m_pAddLeftLegAction;
    QAction* m_pAddHeadAction;
    QMenu* m_pMirrorMenu;
    QAction* m_pLeftArmToRight;
    QAction* m_pRightArmToLeft;
    QAction* m_pRightLegToLeft;
    QAction* m_pLeftLegToRight;
    QMenu* m_curveMenu;
    QAction* m_lineCurveAction;
    QAction* m_bezierCurveAction;
    TipWidget* m_mirrorTip;
    TipWidget* m_reverseTip;
    WidgetTipDialog* m_tipDialog;
    QMenu* m_pReverseMenu;
    QAction* m_pReverseFull;
    QAction* m_pReverseHead;
    QAction* m_pReverseArm;
    QAction* m_pReverseLeg;

};

#endif // BEHAVIORTABLE_H
