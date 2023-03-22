#ifndef FRAMEINTERVALEDITOR_H
#define FRAMEINTERVALEDITOR_H

#include <QDialog>

namespace Ui {
class FrameIntervalEditor;
}

class FrameIntervalEditor : public QDialog
{
    Q_OBJECT

public:
    explicit FrameIntervalEditor(bool bAdd = true, QWidget *parent = nullptr);
    ~FrameIntervalEditor();

signals:
    void sigIntervalChange(bool bAdd,int interval);

private slots:
    void on_frameCount_textEdited(const QString &arg1);
    void on_cancel_clicked();
    void on_ok_clicked();

private:
    Ui::FrameIntervalEditor *ui;
    bool add_;
};

#endif // FRAMEINTERVALEDITOR_H
