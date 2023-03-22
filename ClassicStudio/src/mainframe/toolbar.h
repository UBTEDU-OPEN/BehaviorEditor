#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>

namespace Ui {
class ToolBar;
}

class ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);
    ~ToolBar();
    void connectionStateChanged(int state ,QString sn);
    void chargingStateChanged(int inCharging);
    void batteryPercentChanged(int percent);
    void volumeChanged(int volume);

signals:
    void robotBtnClicked();
    void sigOpenFile();
    void sigNewFile();
    void sigSaveFile();
    void sigSaveAs();
    void sigHelp();

private:
    void configStyle();
    void initConnections();

private:
    Ui::ToolBar *ui;
    int charging_;
    int batteryPercent_;
    int volume_;
};

#endif // TOOLBAR_H
