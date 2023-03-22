#ifndef CONNECTEDWIDGET_H
#define CONNECTEDWIDGET_H

#include <QWidget>
#include<QString>

namespace Ui {
class ConnectedWidget;
}

class ConnectedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectedWidget(QWidget *parent = nullptr);
    ~ConnectedWidget();
    void setSn(QString sn);
signals:
    void disconnectBtnClicked();

private slots:
    void on_disconnectBtn_clicked();

private:
    Ui::ConnectedWidget *ui;
};

#endif // CONNECTEDWIDGET_H
