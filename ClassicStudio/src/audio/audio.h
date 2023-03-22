#ifndef AUDIO_H
#define AUDIO_H

#include <QString>
#include <QList>
#include <QVector>
#include <QMap>
#include <QSharedPointer>

#include "audio_global.h"

struct AUDIO_EXPORT AudioInfo
{
    int64_t duration;
    int64_t bitrate;
    int sampleRate;
    int channels;

    QVector<uint8_t> samples;
    uint8_t minSampleValue;
    uint8_t maxSampleValue;
};


class AUDIO_EXPORT Audio
{
public:
    /**
     * 获取音频文件信息
     *
     * @param[in]  filePath  音频文件路径
     * @param[out] info      音频文件信息
     * @return               成功返回0
     */
    static int getAudioInfo(const QString &filePath, AudioInfo &info);

    static std::shared_ptr<AudioInfo> getAudioInfo(const QString &filePath);

    static void printAudiosInfo();

    static void cleanAudiosInfo();

private:
    Audio();

    static std::map<QString, std::shared_ptr<AudioInfo>> audiosInfo_;
};

#endif // AUDIO_H
