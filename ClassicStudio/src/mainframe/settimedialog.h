#ifndef SETTIMEDIALOG_H
#define SETTIMEDIALOG_H

#include <QDialog>
#include<QCloseEvent>
#include<QString>

namespace Ui {
class SetTimeDialog;
}

class SetTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetTimeDialog(QWidget *parent = nullptr);
    ~SetTimeDialog();
    void closeEvent(QCloseEvent *);

signals:
    void closed();
    void valueChange(int value);
private:
    Ui::SetTimeDialog *ui;
};

#endif // SETTIMEDIALOG_H
