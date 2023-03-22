#include "frameratesettingswidget.h"
#include "ui_frameratesettingswidget.h"

FrameRateSettingsWidget::FrameRateSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameRateSettingsWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(ui->spinBox,QOverload<int>::of(&SpinBox::valueChanged),[this](int newDivision){
        division_ = newDivision;
    });
}

FrameRateSettingsWidget::~FrameRateSettingsWidget()
{
    delete ui;
}

void FrameRateSettingsWidget::setDivision(int division)
{
    division_ = division;
    ui->spinBox->setValue(division);
}
