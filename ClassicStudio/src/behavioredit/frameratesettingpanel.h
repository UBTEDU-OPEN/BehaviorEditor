#ifndef FRAMERATESETTINGPANEL_H
#define FRAMERATESETTINGPANEL_H

#include <QWidget>

namespace Ui {
class FrameRateSettingPanel;
}

class FrameRateSettingPanel : public QWidget
{
    Q_OBJECT

public:
    static const int kMinRate;
    static const int kMaxRate;

public:
    explicit FrameRateSettingPanel(int fps, QWidget *parent = nullptr);
    ~FrameRateSettingPanel();

public slots:
    void onCancelClicked();
    void onOKClicked();
    void onMinusClicked();
    void onAddClicked();
    void onFrameRateSliderValueChanged();

signals:
    void frameRateChanged(int value);
    void frameRateSet(int value);
    void frameRateReset();

private:
    Ui::FrameRateSettingPanel *ui;
};

#endif // FRAMERATESETTINGPANEL_H
