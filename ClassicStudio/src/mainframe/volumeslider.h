#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QDialog>

namespace Ui {
class VolumeSlider;
}

class VolumeSlider : public QDialog
{
    Q_OBJECT

public:
    explicit VolumeSlider(int volume, QWidget *parent = nullptr);
    ~VolumeSlider();

signals:
    void volumeChanged(int);

protected:
    void paintEvent(QPaintEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::VolumeSlider *ui;
    int oldValue_;
};

#endif // VOLUMESLIDER_H
