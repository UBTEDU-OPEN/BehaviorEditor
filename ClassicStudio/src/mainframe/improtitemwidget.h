#ifndef IMPROTITEMWIDGET_H
#define IMPROTITEMWIDGET_H

#include <QWidget>
#include<QListWidgetItem>
#include<QFileInfo>
#include<QTimer>

namespace Ui {
class ImprotItemWidget;
}

class ImprotItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImprotItemWidget(QWidget *parent = nullptr);
    ~ImprotItemWidget();
    void setItem(QListWidgetItem *item);
    QListWidgetItem* getItem();
    void setFileInfo(QFileInfo *fileInfo);
    QFileInfo* getFileInfo();
    bool isChecked();
    void setChecked(bool checked);
    void startImport();
    void stopImport();
signals:
    void importSuccess();
private slots:
    void importTimerOut();
    void sendFileComplate(bool result);

private:
    Ui::ImprotItemWidget *ui;
    QListWidgetItem *mItem;
    QFileInfo *mFileInfo;

    QString byteToString(qint64 size);
    QTimer *mTimer = NULL;
};

#endif // IMPROTITEMWIDGET_H
