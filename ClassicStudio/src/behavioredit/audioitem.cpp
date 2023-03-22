#include "audioitem.h"

#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QPen>
#include <QStyleOption>
#include <QStyle>
#include <QFileInfo>
#include <QDebug>

AudioItem::AudioItem(QString filePath, QWidget *parent)
    : QWidget(parent)
    , filePath_(filePath)
    , selectionState_(Unselected)
    , startDragPos_(0)
    , updateSampleThread_(nullptr)
    , updatingSample_(false)
    , playPos_(0)
    , audioInfo_(nullptr)
{
//    setMouseTracking(true);
    setFixedHeight(kAudioItemHeight);
    setVisible(false);
    audioInfo_ = Audio::getAudioInfo(filePath_);
    updateAudioSamples();
    setStyleSheet("AudioItem {background:#C0C0C0;} AudioItem:hover{background:#00FF00;}");

    connect(this, &AudioItem::calculatePaintSamplesFinished, [this]() {
        update();
    });
}

AudioItem::AudioItem(QWidget *parent)
    : QWidget(parent)
    , selectionState_(Unselected)
    , startDragPos_(0)
    , updateSampleThread_(nullptr)
    , updatingSample_(false)
    , playPos_(0)
    , audioInfo_(nullptr)
{
    setFixedHeight(42);

//    setStyleSheet("AudioItem {background: rgba(61,135,162,76);}");
    setAttribute(Qt::WA_TranslucentBackground);

    connect(this, &AudioItem::calculatePaintSamplesFinished, [this]() {
        update();
    });
}



AudioItem::~AudioItem()
{
    if (updateSampleThread_ && updateSampleThread_->joinable()) {
        updateSampleThread_->join();
        delete updateSampleThread_;
        updateSampleThread_ = nullptr;
    }
    audioInfo_.reset();
    Audio::cleanAudiosInfo();
    Audio::printAudiosInfo();
}

QString AudioItem::getFilePath() const
{
    return filePath_;
}

void AudioItem::setFilePath(const QString& newFilePath)
{
    filePath_ = newFilePath;
    audioInfo_ = Audio::getAudioInfo(filePath_);
    updateAudioSamples();
}

int AudioItem::getPlayTime() const
{
    return static_cast<int>(audioInfo_->duration);
}

int AudioItem::getStartDragPos() const
{
    return startDragPos_;
}

void AudioItem::setSelectionState(AudioItem::SelectionState state)
{
    selectionState_ = state;
    update();
}

void AudioItem::updateToopTip()
{
    QString info;
    auto fileName = filePath_;
    QFileInfo fileInfo(fileName);
    fileName = fileInfo.baseName();
    info = "name: " + fileName + "\n";
    int sec = audioInfo_->duration/1000;
    int min = sec / 60;
    sec = sec % 60;
    info = info + QString("duration: %1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    setToolTip(info);
}

void AudioItem::updateAudioSamples()
{
    if (updateSampleThread_ && updateSampleThread_->joinable()) {
        updateSampleThread_->join();
        delete updateSampleThread_;
        updateSampleThread_ = nullptr;
    }
    updateSampleThread_ = new std::thread([this]() {
        updatingSample_ = true;
        audioInfo_.reset();
        if (!filePath_.isEmpty()) {
            audioInfo_ = Audio::getAudioInfo(filePath_);
        }
        calculatePaintSamples();
        updatingSample_ = false;
    });
}

void AudioItem::updateAudioPaintSamples()
{
    if (updateSampleThread_ && updateSampleThread_->joinable()) {
        updateSampleThread_->join();
        delete updateSampleThread_;
        updateSampleThread_ = nullptr;
    }
    updateSampleThread_ = new std::thread([this]() {
        updatingSample_ = true;
        calculatePaintSamples();
        updatingSample_ = false;
    });
}

void AudioItem::paintEvent(QPaintEvent *e)
{
//    qDebug() << "AudioItem::paintEvent e.region" << e->region().boundingRect();
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    if (selectionState_ == Selected) {
        p.fillRect(e->rect(), QBrush(Qt::blue));
    }
    if (!updatingSample_) {
        auto start = paintSampleRects_.lowerBound(e->region().boundingRect().x());
        if (start != paintSampleRects_.begin()) {
            --start;
        }
        auto end = paintSampleRects_.lowerBound(e->region().boundingRect().x() + e->region().boundingRect().width());
        for (auto itr = start; itr != end; ++itr) {
            if (itr == paintSampleRects_.end()) {
                break;
            }
            p.fillRect(itr.value(), QBrush(QColor(0x5d,0xfc,0xff)));
        }
    }
}

void AudioItem::calculatePaintSamples()
{
    paintSampleRects_.clear();
    if (audioInfo_ && audioInfo_->maxSampleValue != 0) {
        const int paintInterval = 5;
        const int sampleRectWidth = 3;
        int paintSamplesNum = width() / paintInterval;
        int sampleInterval = audioInfo_->samples.count() / paintSamplesNum;
        int maxH = height() * 3 / 4 ;
        int middleY = height() / 2;
        int rectIndex = 0;
        for (int i = 0; i < audioInfo_->samples.count(); ++i) {
            if (i % sampleInterval == 0) {
                int h =  maxH * audioInfo_->samples.at(i) / audioInfo_->maxSampleValue;
                QRect paintSampleRect;
                paintSampleRect.setX(rectIndex * paintInterval + 1);
                paintSampleRect.setY(middleY - h);
                paintSampleRect.setWidth(sampleRectWidth);
                paintSampleRect.setHeight(2 * h);
                paintSampleRects_.insert(paintSampleRect.x(), paintSampleRect);
                ++rectIndex;
            }
        }
    }
    emit calculatePaintSamplesFinished();
}
