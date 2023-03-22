#ifndef AUDIOITEM_H
#define AUDIOITEM_H

#include <QWidget>
#include <QString>
#include <QMap>
#include <thread>

#include "audio.h"

typedef struct _ClassicAudioInfo{
    QString filePath;
    int playTime;
    int id;
} ClassicAudioInfo;

class AudioItem : public QWidget
{
    Q_OBJECT

public:
    static const int kAudioItemHeight = 26;

    enum SelectionState {
        Selected = 0,
        Unselected,
        StateCount
    };

public:
    explicit AudioItem(QString filePath, QWidget *parent = nullptr);
    explicit AudioItem(QWidget *parent);
    ~AudioItem();

    QString getFilePath() const;
    void setFilePath(const QString& newFilePath);
    int getPlayTime() const;

    int getStartDragPos() const;
    void storeStartDragPos() { startDragPos_ = x(); }

    SelectionState getSelectionState() const { return selectionState_; }
    void setSelectionState(SelectionState state);

    void setPlayPosition(int t) { playPos_ = t; }
    int getPlayPosition() const { return playPos_; }

    void updateToopTip();
    void updateAudioSamples();
    void updateAudioPaintSamples();

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    void calculatePaintSamples();

signals:
    void calculatePaintSamplesFinished();

private:
    QString filePath_;
    SelectionState selectionState_;
    int startDragPos_;

    std::thread *updateSampleThread_;
    QMap<int, QRect> paintSampleRects_;
    std::shared_ptr<AudioInfo> audioInfo_;
    bool updatingSample_;

    int playPos_;
};

#endif // AUDIOITEM_H
