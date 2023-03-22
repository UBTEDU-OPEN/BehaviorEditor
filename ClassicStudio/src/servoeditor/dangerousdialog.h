#ifndef DANGEROUSDIALOG_H
#define DANGEROUSDIALOG_H

#include <QWidget>
#include<QDialog>
namespace Ui {
class DangerousDialog;
}

class DangerousDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DangerousDialog(QWidget *parent = nullptr);
    ~DangerousDialog();
signals:
    void sigSure();

private slots:
    void on_closeBtn_clicked();

    void on_iKnowBtn_clicked();

    void on_noMoreTipsBtn_clicked();

private:
    Ui::DangerousDialog *ui;
};

#endif // DANGEROUSDIALOG_H
