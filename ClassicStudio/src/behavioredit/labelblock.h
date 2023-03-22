#ifndef LABELBLOCK_H
#define LABELBLOCK_H

#include <QWidget>
#include <QPoint>
#include <QMenu>
#include <QAction>

const int kStaticLabelIndex = 0;

class LabelBlock : public QWidget
{
    Q_OBJECT
public:
    explicit LabelBlock(QWidget *parent = nullptr);

    void setContent(const QString& content){content_ = content;}
    void setFrameIndex(const int frameIndex){frameIndex_ = frameIndex;}
    int getFrameIndex(){return frameIndex_;}
    int getIndex(){return index_;}

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

public slots:
    void onContentChanged(int,QString);
    void onEdit();
    void onDelete();

signals:
    void sigMouseMove(int,QPoint);
    void sigMouseRelease(int,QPoint);
    void sigDelete(int);
    void sigFrameIndexChange(int,int);

private:
    void openEditDialog();

private:
    QString content_;
    int frameIndex_;
    QMenu* menu_;
    QAction* edit_;
    QAction* delete_;
    int index_;

    static int sIndex;
};

#endif // LABELBLOCK_H
