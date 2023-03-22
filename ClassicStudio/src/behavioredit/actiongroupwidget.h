#ifndef ACTIONGROUPWIDGET_H
#define ACTIONGROUPWIDGET_H

#include <QWidget>
#include <QMap>
#include <QMenu>
#include <QAction>

class ActionFrameWidget;
class DistanceWidget;

namespace Ui {
class ActionGroupWidget;
}



class ActionGroupWidget : public QWidget
{
    Q_OBJECT

public:
    static const int kFrameWidgetWidth = 14;
    explicit ActionGroupWidget(QString groupName, QSet<int> frames, QWidget *parent = nullptr);
    ~ActionGroupWidget();
    bool containsFrameIndex(int frameIndex);
    QList<int> frameList();
    int groupRange();
    void deleteFrameWidget(int frameIndex);
    void setGroupIndex(int index);
    int groupIndex() { return groupIndex_; }
    QString groupName() { return groupName_; }
    ActionGroupWidget* previousWidget() { return previousGroupWidget_; }
    void setPreviousWidget(ActionGroupWidget *previous) { previousGroupWidget_ = previous; }
    void clearSelection();
    void selectFrameWidget(int frameIndex);
    void setGroupSelected(bool selected);

public:
    static double frameDistance_;
    static QMap<int,ActionFrameWidget*> selectedFrameWidgets_;
    static ActionGroupWidget* groupOfSelectedFrames_;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    //用于提示后面的group跟随
    void groupWidgetMoved(int,int);
    //用于在behaviorcontainer中显示或取消显示distancewidget
    void groupMoved();
    void groupReleased();
    //用于更新显示
    void groupPressed(int,bool);
    void framePressed(int,int,bool);
    //
    void combineGroups();

private:
    void init(QSet<int> frames);
    void addFrame(int frameIndex);
    void addFrames(QList<int> frames);
    ActionFrameWidget* frameWidgetAt(QPoint pos);
    void onCopy();
    void onPaste();
    void onDelete();
    void onRename();
    void onSaveAsTemplate();
    void onSplitGroup();
    void onCombineGroups();

    void onCopyFrame();
    void onPasteFrame();
    void onDeleteFrame();
    void onCustomContextMenuRequested(const QPoint &pos);
    ActionFrameWidget* previousFrameWidget(int leftSide);
    void showDistanceWidget();
    void moveGroupAndOtherFrames(QMap<int,ActionFrameWidget*> frameWidgets, int delta);
    void moveFrameWidgets(QMap<int,ActionFrameWidget*> frameWidgets, int delta);
    void notifyGroupsBehindMove();
    int calcLeftBound();
    void resetSelectedFrames();
    void resetGroupWidth();
    void boxChooseFrames();


private:
    Ui::ActionGroupWidget *ui;
    int groupIndex_ = 0; //使用起始frameindex作为组序号
    QString groupName_;
    ActionGroupWidget   *previousGroupWidget_ = nullptr;
    QMap<int,ActionFrameWidget*> frameWidgets_;

    DistanceWidget  *distanceWidget_ = nullptr;
    QMenu   *groupMenu_ = nullptr;
    QMenu   *frameMenu_ = nullptr;
    QAction *copyGroupAction_ = nullptr;
    QAction *pasteGroupAction_ = nullptr;
    QAction *copyFrameAction_ = nullptr;
    QAction *pasteFrameAction_ = nullptr;
    QAction *deleteFrameAction_ = nullptr;
    QAction *splitGroupAction_ = nullptr;
    QAction *combineGroupsAction_ = nullptr;

    bool groupPressed_ = false;
    bool framePressed_ = false;
    int rightPressIndex_ = 0;
    int frameMoveDelta_ = 0;
    int groupMoveDelta_ = 0;
    int groupRightSide_ = 0;

    bool drawSelectionRect_ = false;
    QPoint pos1;
    QPoint pos2;

    QPoint pressPos_;
    QPoint lastPos_;
    bool mouseMoved_;
    bool ctrlMotion_;
    bool frameAreaPressed_;
    ActionFrameWidget* pressedFrameWidget_;
    QMap<int,ActionFrameWidget*> tempFrameWidgets_;
};

#endif // ACTIONGROUPWIDGET_H
