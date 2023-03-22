#ifndef INPUTSNWIDGET_H
#define INPUTSNWIDGET_H

#include <QWidget>
#include<QString>
#include<QKeyEvent>
#include"classstudiosettings.h"

namespace Ui {
class InputSnWidget;
}

class InputSnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InputSnWidget(QWidget *parent = nullptr);
    ~InputSnWidget();
    void setErroeView(int errorCode);
    bool eventFilter(QObject *obj, QEvent *event) override;
signals:
    void connectBtnClicked(const QString &sn);
private slots:
    void onTextChange();
    void on_connectBtn_clicked();

private:
    Ui::InputSnWidget *ui;

};

#endif // INPUTSNWIDGET_H
