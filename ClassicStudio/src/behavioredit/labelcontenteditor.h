#ifndef LABELCONTENTEDITOR_H
#define LABELCONTENTEDITOR_H

#include <QDialog>

namespace Ui {
class LabelContentEditor;
}

class LabelContentEditor : public QDialog
{
    Q_OBJECT

public:
    LabelContentEditor(const int frameIndex, const int fps, const QString content, QWidget *parent = nullptr);
    ~LabelContentEditor();

private slots:
    void on_ok_clicked();
    void on_cancel_clicked();
    void onTextChanged();
    void onFrameChanged();

signals:
    void sigContentChanged(int,QString);

private:
    QString calcTimeStamp(int);

private:
    Ui::LabelContentEditor *ui;
    int frameIndex_;
    int fps_;
};

#endif // LABELCONTENTEDITOR_H
