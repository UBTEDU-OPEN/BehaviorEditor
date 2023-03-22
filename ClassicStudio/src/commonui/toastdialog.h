#ifndef TOASTDIALOG_H
#define TOASTDIALOG_H
#include "commonui_global.h"
#include <QDialog>

namespace Ui {
class ToastDialog;
}

class COMMONUI_EXPORT ToastDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ToastDialog(QWidget *parent = nullptr);
    void setDisplayText(QString text);
    ~ToastDialog();

private:
    Ui::ToastDialog *ui;
};

#endif // TOASTDIALOG_H
