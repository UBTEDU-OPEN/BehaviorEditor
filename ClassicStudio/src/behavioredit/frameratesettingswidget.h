#ifndef FRAMERATESETTINGSWIDGET_H
#define FRAMERATESETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class FrameRateSettingsWidget;
}

class FrameRateSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FrameRateSettingsWidget(QWidget *parent = nullptr);
    ~FrameRateSettingsWidget();
    void setDivision(int division);
    int division() { return division_; }

private:
    Ui::FrameRateSettingsWidget *ui;
    int division_;
};

#endif // FRAMERATESETTINGSWIDGET_H
