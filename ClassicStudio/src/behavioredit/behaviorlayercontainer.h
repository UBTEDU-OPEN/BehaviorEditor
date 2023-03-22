#ifndef BEHAVIORLAYERCONTAINER_H
#define BEHAVIORLAYERCONTAINER_H

#include <QWidget>
#include <QMap>
#include <QPair>
#include <QVector>
#include <QAction>
#include <QMenu>

#include "layerbgwidget.h"
#include "servoitem.h"
#include "audioitem.h"
#include "actiongroupwidget.h"
#include "distancewidget.h"
#include "musicwidget.h"


class QMediaPlayer;

class BehaviorLayerContainer : public QWidget
{
    Q_OBJECT

public:
    static const QString kServoItemDataFormat;
    static const QString kMusicItemDataFormat;
    static const QString kLEDItemDataFormat;

public:
    explicit BehaviorLayerContainer(int fn, double fd, QWidget *parent = nullptr);
    ~BehaviorLayerContainer();
    bool onNewBehavior();
    void onFrameNumberChanged(int fn);
    void setScaleFactor(double sf);
    void setFrameRate(int fps);

    void startPlay(int startFrame);
    void stopPlay();
    void onDivisionChanged();


public slots:
    void onRightMenu(const QPoint&);
    void onCopy();
    void onPaste();
    void onDelete();
    void onKeyEvent(QKeyEvent*);
    void addMusic(const QString&);
    void addFileToFrame(int frameIndex, const QString &filePath);
    void onGroupChanged(QMap<int,QPair<QString,QSet<int>>> groups,int selectFrameIndex);
    void onMusiceDelete(const QString& musicName);
    void onGroupMoved();
    void onGroupReleased();
    void onGroupPressed(int groupIndex, bool ctrl);
    void onFramePressed(int groupIndex, int frameIndex, bool ctrl);
    void onMoveGroupsBehind(QSet<int>,int);
    void onChangeGroup(int originGroupIndex, QSet<int> newFrameIndexes);
    void onCreateGroup(QString groupName, QSet<int> frameIndexes);
    void onDeleteGroup(int groupIndex);
    void onMusicSelected(const QString& musicName, bool selected);
    void onCombineGroups();

    void onFramesSelectionChanged(QSet<int> frames);
    void onGroupsSelectionChanged(QSet<int> groups);
    void onClearMusicSelection();
    void onRevertGroups(QMap<QString,QSet<int>> fullGroups);
    void onTimeLineChanged();

protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void init();
    void initConfiguration();
    void updateContent();
    bool loadAudioData();
    inline double frameDistance() const;

    // audio item:
    void plotAudioItem(AudioItem* item, double frameDis);
    int getMusicDuration(const QString &filePath) const;
    void deleteMusic();
    void playAudioAtFrame(int frame);

    void initGroupWidgetConnections(ActionGroupWidget *groupWidget);
    ActionGroupWidget *getPreviousGroupWidget(int frameIndex);
    ActionGroupWidget *insertGroupWidget(QString groupName, QSet<int> frames);
    void moveGroupsBehind(int groupId, int delta);
    void updateGroupWidgetPrevious();
    void selectFrameWidget(int frameIndex);
    void showDistanceWidget();
    ActionGroupWidget* groupWidgetAt(QPoint pos);
    ActionGroupWidget* getGroupByFrame(int frameIndex);
    void clearAllSelection();

    int calcLeftBound(ActionGroupWidget* groupWidget);

    void boxChooseGroups();


signals:
    void changeFramePointer(int);
    void stateChanged(int newState);
    void zoomIn();
    void zoomOut();

private:
    enum SelectionType {
        kSelectionNone = 0,
        kSelectionMusic,
        kSelectionFrames,
        kSelectionGroups
    };
    QVector<LayerBgWidget *> layerBgWidgets_;
    // music items: start time -> expression item

    int frameNumber_;
    double frameDistanceBase_;
    double scaleFactor_;

    QTimer *audioPlayTimer_;
    QMediaPlayer *mediaPlayer_;

    QMenu* mainMenu_;
    QAction* copyAction_;
    QAction* pasteAction_;
    QAction* deleteAction_;

    int fullLayerId_{0};
    QMap<int,int> bodyIdToLayerId_;
    int audioLayerId_{0};
    int layerCount_{0};
    QMap<int,ActionGroupWidget*> actionGroups_;
    MusicWidget* musicWidget_;
    DistanceWidget  *distanceWidget_;
    QSet<ActionGroupWidget*> selectedGroups_;
    int selectedFrameIndex_;
    SelectionType selectionType_;

    bool drawSelectionRect_ = false;
    QPoint pos1;
    QPoint pos2;


    bool mouseMoved_; //如果移动小于5px，则判定没有移动
    bool ctrlMotion_;
    ActionGroupWidget* pressedGroupWidget_;
    QPoint pressPos_;
    QPoint lastPos_;
    QMap<int,ActionGroupWidget*> tempCreatedWidgets_;
    QMap<int,ActionGroupWidget*> selectedGroupWidgets_;
    bool mousePressed_;
};

#endif // BEHAVIORLAYERCONTAINER_H
