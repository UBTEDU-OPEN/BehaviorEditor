#ifndef LAYERBGWIDGET_H
#define LAYERBGWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class LayerBgWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LayerBgWidget(int layerId, QString layerName, QWidget* parent = nullptr);
    QString getLayerName() { return layerName_; }
    int getLayerId() { return layerId_; }
    void setDrawRect(bool draw,QPoint pos);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *e) override;

signals:
    void sigRightButtonPressed(int);
    void sigLeftButtonReleased();

private:
    int layerId_;
    QString layerName_;

    bool drawSelectionRect_ = false;
    QPoint pos1;
    QPoint pos2;
};

#endif // LAYERBGWIDGET_H
