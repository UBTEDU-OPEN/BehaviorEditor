#include "frameratesettingpanel.h"
#include "ui_frameratesettingpanel.h"

const int FrameRateSettingPanel::kMinRate = 1;
const int FrameRateSettingPanel::kMaxRate = 50;

FrameRateSettingPanel::FrameRateSettingPanel(int fps, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameRateSettingPanel)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->setupUi(this);

    connect(ui->minusBtn, &QPushButton::clicked,
            this, &FrameRateSettingPanel::onMinusClicked);
    connect(ui->addBtn, &QPushButton::clicked,
            this, &FrameRateSettingPanel::onAddClicked);
    connect(ui->frameRateSlider, &QSlider::valueChanged,
            this, &FrameRateSettingPanel::onFrameRateSliderValueChanged);
    connect(ui->cancelBtn, &QPushButton::clicked,
            this, &FrameRateSettingPanel::onCancelClicked);
    connect(ui->OKBtn, &QPushButton::clicked,
            this, &FrameRateSettingPanel::onOKClicked);

    ui->frameRateSlider->setRange(kMinRate, kMaxRate);
    if (fps < kMinRate) {
        fps = kMinRate;
    }
    if (fps > kMaxRate) {
        fps = kMaxRate;
    }
    ui->frameRateSlider->setValue(fps);
}

FrameRateSettingPanel::~FrameRateSettingPanel()
{
    delete ui;
}

void FrameRateSettingPanel::onCancelClicked()
{
    hide();
    emit frameRateReset();
    close();
}

void FrameRateSettingPanel::onOKClicked()
{
    hide();
    emit frameRateSet(ui->frameRateSlider->value());
    close();
}

void FrameRateSettingPanel::onMinusClicked()
{
    int val = ui->frameRateSlider->value();
    if (--val < kMinRate) {
        val = kMinRate;
    }
    ui->frameRateSlider->setValue(val);
}

void FrameRateSettingPanel::onAddClicked()
{
    int val = ui->frameRateSlider->value();
    if (++val > kMaxRate) {
        val = kMaxRate;
    }
    ui->frameRateSlider->setValue(val);
}

void FrameRateSettingPanel::onFrameRateSliderValueChanged()
{
    int fps = ui->frameRateSlider->value();
    ui->frameRate->setText(QString("%1 fps").arg(fps));
    double spf = 1 / static_cast<double>(fps);
    ui->frameSpeed->setText(QString("%1 spf").arg(spf, 2));
    emit frameRateChanged(ui->frameRateSlider->value());
}
