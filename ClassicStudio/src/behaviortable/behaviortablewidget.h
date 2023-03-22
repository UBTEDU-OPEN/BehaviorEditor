#ifndef BEHAVIORTABLEWIDGET_H
#define BEHAVIORTABLEWIDGET_H

#include <QTableView>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QItemSelection>
#include <QMap>
#include <QSet>
#include <QVector>
#include <QMenu>
#include <QAction>

class HorizontalHeaderView;
class BehaviorTableDelegate;


class BehaviorTableWidget : public QTableView {
    Q_OBJECT

public:
    explicit BehaviorTableWidget(QWidget * parent = nullptr);
    ~BehaviorTableWidget();
    void onSetCurve(int curveType);
    void onSetServosValue(double value);
    void clearCheckState();
    void clearAllSelection(bool notify = true);
    void onSelectRow(int row);

signals:
    void sigSameServoValue(bool isSame,double sameValue);
    void sigSelcectedChanged(QMap<int, QMap<int,double>>);
    void sigEnableReverseAndMirror(bool);
    void sigEnableCurve(bool);
    void sigEnableDelete(bool);
    void sigServosValueChanged(const QMap<int,QMap<int,double>>& servosValue);
    void sigCurveChanged(QSet<int> frames, int curveType);
    void sigChangeSelectAllState(int);
    void sigMirror(int);
    void sigReverse(int);
    void sigDeleteSelected();

public slots:
    void onSelectAllStateChanged(bool isChecked);
    void onBehaviorEditSelected(const QMap<int, QMap<int,double>> &selectedServos);

    void onClearSelection();
    void deleteSelection();

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QTableView *frozenTableView;
    QStandardItemModel* m_model;
    HorizontalHeaderView* m_hHeader;
    BehaviorTableDelegate* m_tableDelegate;
    QModelIndex m_openedEditorIndex;
    QVector<QSet<int>> m_rowServos;
    bool m_isBehaviorSelect;
    QModelIndex m_pressPosIndex;
    QModelIndex m_frozenPressPosIndex;
    QModelIndex m_rightClickIndex;
    int m_firstVisibleColumn;
    int m_frozenVisibleWidth;
    int m_headerPressIndex;
    int m_pressItemCheckState;
    QMap<int,QPair<double,double>> m_servosRange;
    QMenu* m_pMirrorMenu;
    QAction* m_pLeftArmToRight;
    QAction* m_pRightArmToLeft;
    QAction* m_pRightLegToLeft;
    QAction* m_pLeftLegToRight;
    QMenu* m_pReverseMenu;
    QAction* m_pReverseFull;
    QAction* m_pReverseHead;
    QAction* m_pReverseArm;
    QAction* m_pReverseLeg;
    QMenu* m_pPostureMenu;
    QAction* m_pStandPos;
    QAction* m_pSquatPos;
    QAction* m_pSpreadArmsPos;
    QMenu* m_pContexMenu;
    QAction* m_pDeleteAction;
    QAction* m_pCopyAction;
    QAction* m_pPasteAction;
    QMap<int, QMap<int,double>> copyData_;

private:
    void initFrozenTable();
    void updateFrozenTableGeometry(); 
    //row,cols
    void updateCheckState(QMap<int,QSet<int>>);

    QString getCurveModeString(int runMode);
    int getCurveMode(QString displayString);


    void selectItems(const QModelIndex& firstIndex, const QModelIndex& secondIndex, bool selectFrozenTable);
    void selectColumns(int firstColumn, int secondColumn);
    void selectRows(const QModelIndex& firstIndex, const QModelIndex& secondIndex);
    void selectAllItems();
    void mirrorLeftArmToRight();
    void mirrorRightArmToLeft();
    void mirrorLeftLegToRight();
    void mirrorRightLegToLeft();
    void reverseFull();
    void reverseHead();
    void reverseArm();
    void reverseLeg();
    void setStandPos();
    void setSquatPos();
    void setSpreadArmsPos();

    void copySelection();
    void pasteSelection();
    void initContextMenu();
    void configStyle();

    void onPasteToCurrentRow();
    void onPasteToPreviousRow(); //insert
    void onPasteToNextRow(); //insert

private slots:
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

};

#endif // BEHAVIORTABLEWIDGET_H
