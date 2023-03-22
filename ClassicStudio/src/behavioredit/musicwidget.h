#ifndef MUSICWIDGET_H
#define MUSICWIDGET_H

#include <QWidget>
#include <QMenu>
#include "audioitem.h"

namespace Ui {
class MusicWidget;
}

class MusicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MusicWidget(const QString& filePath, QWidget *parent = nullptr);
    ~MusicWidget();
    AudioItem* getAudioItem();
    void setSelected(bool selected);

signals:
    void musicSelected(const QString&,bool);
    void deleteMusic(const QString&);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    Ui::MusicWidget *ui;
    QMenu* audioMenu_;
};

#endif // MUSICWIDGET_H
