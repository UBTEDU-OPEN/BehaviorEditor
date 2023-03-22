#ifndef TEMPLATEITEM_H
#define TEMPLATEITEM_H

#include <QWidget>
#include<QTimer>

namespace Ui {
class TemplateItem;
}

class TemplateItem : public QWidget
{
    Q_OBJECT

public:
    explicit TemplateItem(QString actionName,QWidget *parent = nullptr);
    ~TemplateItem();
    QString getActionName();
    void setActionName(QString actionName);
    void setSelected(bool selected);
    void setFiles(QList<QString> files);
    bool eventFilter(QObject *watched, QEvent *event);
    void setIndex(int index);
    int getIndex();
signals:
    void sigAddCurFrame(QString fileName);
    void selectedItem(int index);
    void longPressEvent(QPoint point,QString filePath);
    void longPressReleaseEvent(QPoint point,QString filePath);
    void longPressMoveEvent(QPoint point);

private:
    Ui::TemplateItem *ui;
    QString mActionName;
    QList<QString> m_files;
    int mIndex;
    QTimer *mTimer;
    QPoint mPressPoint;
    bool longPressStarted = false;
    void startLongPress();
    void stopLongPress();
    void onLongPress();

};

#endif // TEMPLATEITEM_H
