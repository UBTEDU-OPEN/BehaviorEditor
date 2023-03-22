#include "searchingwidget.h"
#include "ui_searchingwidget.h"
#include<QPalette>
#include<QMovie>

SearchingWidget::SearchingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchingWidget)
{
    ui->setupUi(this);
    QMovie *movie = new QMovie(this);
    movie->setFileName(":/images/res/images/loading.gif");
    ui->loadingLabel->setMovie(movie);
    ui->loadingLabel->setFixedSize(70,70);
    ui->loadingLabel->setScaledContents(true);
    movie->setScaledSize(QSize(ui->loadingLabel->width(),ui->loadingLabel->height()));
    movie->start();
}

SearchingWidget::~SearchingWidget()
{
    delete ui;
}
