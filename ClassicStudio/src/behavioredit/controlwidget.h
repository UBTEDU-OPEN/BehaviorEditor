#ifndef CONTROL_WIDGET_H
#define CONTROL_WIDGET_H

#include <QWidget>
#include <QList>
#include <QStandardItemModel>
#include <QItemSelection>
#include <QPushButton>

#include "frameratesettingpanel.h"
#include "datacenter.h"

namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = nullptr);
    ~ControlWidget() override;
    void onPlayerStateChanged(DataCenter::PlayActionType playType);
    void onDurationChanged(int durationSec);
    void onPlayPos(int ms);
    void onPlayableStateChanged();
    void onZoomIn();
    void onZoomOut();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void init();
    void onScaleValueChanged(int value);
    void onFrameRateSettingBtnClicked();
    void onPlayClicked();
    void onStopClicked();
    void onPlayActionClicked();
    void onStopActionClicked();
    void onPlayMusicClicked();
    void onStopMusicClicked();
    void onAddMusicClicked();
    void scaleUp();
    void scaleDown();

signals:
    // play control:
    void stopClicked();
    void playClicked();
    void playActionClicked();
    void stopActionClicked();
    void playMusicClicked();
    void stopMusicClicked();

    // frame rate:
    void frameRateChanged(int fps);
    void frameIntervalChanged(int frameInterval);
    // scale:
    void scaleValueChanged(int newValue);

private:
    Ui::ControlWidget *ui;
    QPushButton* addMusic_;
    bool actionDisabled_;
    bool musicDisabled_;
};

#endif // CONTROL_WIDGET_H
