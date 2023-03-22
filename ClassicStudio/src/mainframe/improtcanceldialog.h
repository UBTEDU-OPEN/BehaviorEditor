#ifndef IMPROTCANCELDIALOG_H
#define IMPROTCANCELDIALOG_H

#include <QDialog>

namespace Ui {
class ImprotCancelDialog;
}

class ImprotCancelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImprotCancelDialog(QWidget *parent = nullptr);
    ~ImprotCancelDialog();

private slots:
    void on_closeBtn_clicked();

    void on_noBtn_clicked();

    void on_yesBtn_clicked();

signals:
    void cancelImprot();
private:
    Ui::ImprotCancelDialog *ui;
};

#endif // IMPROTCANCELDIALOG_H
