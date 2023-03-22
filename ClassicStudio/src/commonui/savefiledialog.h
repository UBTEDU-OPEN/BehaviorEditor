#ifndef SAVEFILEDIALOG_H
#define SAVEFILEDIALOG_H
#include "commonui_global.h"
#include <QDialog>

namespace Ui {
class SaveFileDialog;
}

class COMMONUI_EXPORT SaveFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveFileDialog(QString behaviorPath,QWidget *parent = nullptr);
    ~SaveFileDialog();

signals:
    void sigSaveFile(QString fileName,QString fileDir);
private slots:
    void on_closeBtn_clicked();

    void on_choosePathBtn_clicked();

    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::SaveFileDialog *ui;
    QString mBehaviorPath;
};

#endif // SAVEFILEDIALOG_H
