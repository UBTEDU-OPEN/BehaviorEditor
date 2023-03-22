#ifndef HORIZONTALHEADERVIEW_H
#define HORIZONTALHEADERVIEW_H

#include <QHeaderView>
#include <QCheckBox>

class HorizontalHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit HorizontalHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr);

signals:
    void sigSelectAll(bool);
    void sigLButtonPressEvent(int);
    void sigLButtonMoveEvent(int);

public slots:
    void onSelectChanged(int);

protected:
    void updateGeometries() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QCheckBox* m_selectAll;
};

#endif // HORIZONTALHEADERVIEW_H
