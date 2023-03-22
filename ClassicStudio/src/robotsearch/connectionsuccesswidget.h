#ifndef CONNECTIONSUCCESSWIDGET_H
#define CONNECTIONSUCCESSWIDGET_H

#include <QWidget>

namespace Ui {
class ConnectionSuccessWidget;
}

class ConnectionSuccessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionSuccessWidget(QWidget *parent = nullptr);
    ~ConnectionSuccessWidget();
signals:
    void successBtnClicked();
private slots:
    void on_successBtn_clicked();

private:
    Ui::ConnectionSuccessWidget *ui;
};

#endif // CONNECTIONSUCCESSWIDGET_H
