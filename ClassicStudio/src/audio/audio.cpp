#include "audio.h"

#include <QUrl>
#include <QDebug>

#ifdef _WIN32
//Windows
#ifdef __cplusplus
extern "C"
{
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#ifdef __cplusplus
}
#endif
#else
// linux:
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#ifdef __cplusplus
}
#endif
#endif

#define MAX_AUDIO_FRAME_SIZE (22050) // 1 second of 22050hz 8bit audio

Audio::Audio()
{
}

std::map<QString, std::shared_ptr<AudioInfo>> Audio::audiosInfo_;

int Audio::getAudioInfo(const QString &filePath, AudioInfo &info)
{
    int ret = -1;
    if (filePath.isEmpty()) {
        qDebug() << "filePath is empty.";
        return ret;
    }

    AVFormatContext	*formatContext = nullptr;
    unsigned int	i, audioStream;
    AVCodecContext	*codecContext = nullptr;
    AVCodec			*codec = nullptr;
    AVPacket		*packet = nullptr;
    AVFrame			*frame = nullptr;
    int             gotPicture;

    av_register_all();
    avformat_network_init();
    formatContext = avformat_alloc_context();

    //Open
    if (avformat_open_input(&formatContext, filePath.toStdString().c_str(), NULL, NULL) != 0) {
        qDebug() << "Couldn't open input stream.";
        return ret;
    }

    // Retrieve stream information
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        qDebug() << "Couldn't find stream information.";
        return ret;
    }
    info.duration = formatContext->duration / 1000;
    info.bitrate = formatContext->bit_rate;
    // Dump valid information onto standard error
    av_dump_format(formatContext, 0, filePath.toStdString().c_str(), false);

    // Find the first audio stream
    audioStream = -1;
    for (i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO){
            audioStream = i;
            break;
        }
    }
    if (audioStream == -1) {
        qDebug() << "Didn't find a audio stream.";
        return ret;
    }

    // Get a pointer to the codec context for the audio stream
    codecContext = formatContext->streams[audioStream]->codec;

    // Find the decoder for the audio stream
    codec = avcodec_find_decoder(codecContext->codec_id);
    if (codec == NULL) {
        qDebug() << "Codec not found.";
        return ret;
    }

    // Open codec
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        qDebug() << "Could not open codec.";
        return ret;
    }

    packet = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_init_packet(packet);
    while (av_read_frame(formatContext, packet) >= 0 && (ret !=0)) {
        if (packet->stream_index == audioStream) {
            frame = av_frame_alloc();
            ret = avcodec_decode_audio4(codecContext, frame, &gotPicture, packet);
            if (ret < 0) {
                qDebug() << "Error in decoding audio frame.";
                av_frame_free(&frame);
            } else {
                qDebug() << "frame info: sample_rate" << frame->sample_rate << " channels:" << frame->channels;
                info.sampleRate = frame->sample_rate, info.channels = frame->channels;
                ret = 0;
                av_frame_free(&frame);
//                break;
            }
        }
    }
    av_free_packet(packet);
    avcodec_close(codecContext);
    avformat_close_input(&formatContext);

//    qDebug() << "\n##### audio info:";
//    qDebug() << "\tname:" << filePath;
//    qDebug() << "\tduration:" << info.duration << " bit rate:" << info.bitrate;
//    qDebug() << "\tsample rate:" << info.sampleRate << " channels:" << info.channels << "\n";

    return ret;
}

std::shared_ptr<AudioInfo> Audio::getAudioInfo(const QString &filePath)
{
//    qDebug() << "Audio::getAudioInfo filePath:" << filePath;
//    qDebug() << "##### before call:";
//    printAudiosInfo();

    auto audioInfo = std::make_shared<AudioInfo>();
    if (filePath.isEmpty()) {
        qDebug() << "filePath is empty.";
        audioInfo.reset();
        return audioInfo;
    }

    auto itr = audiosInfo_.find(filePath);
    if (itr != audiosInfo_.end()) {
        return itr->second;
    }

    AVFormatContext	*formatContext = nullptr;
    unsigned int    i, audioStream;
    AVCodecContext	*codecContext = nullptr;
    AVCodec			*codec = nullptr;
    AVPacket		*packet = nullptr;
    AVFrame			*frame = nullptr;
    av_register_all();
    avformat_network_init();
    formatContext = avformat_alloc_context();
    //Open
    if (avformat_open_input(&formatContext, filePath.toStdString().c_str(), NULL, NULL) != 0) {
        qDebug() << "Couldn't open input stream.";
        audioInfo.reset();
        return audioInfo;
    }
    // Retrieve stream information
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        qDebug() << "Couldn't find stream information.";
        audioInfo.reset();
        return audioInfo;
    }
    audioInfo->duration = formatContext->duration / 1000;
    audioInfo->bitrate = formatContext->bit_rate;
    // Dump valid information onto standard error
    av_dump_format(formatContext, 0, filePath.toStdString().c_str(), false);
    // Find the first audio stream
    audioStream = -1;
    for (i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStream = i;
            break;
        }
    }
    if (audioStream == -1) {
        qDebug() << "Didn't find a audio stream.";
        audioInfo.reset();
        return audioInfo;
    }

    // Get a pointer to the codec context for the audio stream
    codecContext = formatContext->streams[audioStream]->codec;

    // Find the decoder for the audio stream
    codec = avcodec_find_decoder(codecContext->codec_id);
    if (codec == NULL) {
        qDebug() << "Codec not found.";
        audioInfo.reset();
        return audioInfo;
    }

    // Open codec
    if(avcodec_open2(codecContext, codec, NULL) < 0){
        qDebug() << "Could not open codec.";
        audioInfo.reset();
        return audioInfo;
    }

    //Out Audio Param
    uint64_t out_channel_layout = AV_CH_FRONT_CENTER;
//    int out_nb_samples = codecContext->frame_size;
    AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_U8;
    int out_sample_rate = 22050;
//    int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
//    int out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);
    uint8_t *out_buffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE);

    //FIX: Some Codec's Context Information is missing
    int64_t in_channel_layout = av_get_default_channel_layout(codecContext->channels);
    //Swr
    struct SwrContext *au_convert_ctx = swr_alloc();
    au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate,
        in_channel_layout, codecContext->sample_fmt, codecContext->sample_rate, 0, NULL);
    swr_init(au_convert_ctx);

    int firstNotEmptyOutput = true;
    int frame_index = 0;
    packet = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_init_packet(packet);
    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == audioStream) {
            auto ret = avcodec_send_packet(codecContext, packet);
            if (ret < 0) {
                qDebug() << "Error sending a packet for decoding";
                audioInfo.reset();
                return audioInfo;
            }
            frame = av_frame_alloc();
            do {
                ret = avcodec_receive_frame(codecContext, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                   break;
                }
                else if (ret < 0) {
                   qDebug() << "Error during decoding";
                   av_frame_free(&frame);
                   audioInfo.reset();
                   return audioInfo;
                }
                int outputCount = swr_convert(au_convert_ctx, &out_buffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)frame->data, frame->nb_samples);
                if (outputCount > 0 && firstNotEmptyOutput) {
                    audioInfo->maxSampleValue = audioInfo->minSampleValue = out_buffer[0];
                    firstNotEmptyOutput = false;
                }
                for (int i = 0; i < outputCount; ++i) {
                    audioInfo->samples.append(out_buffer[i]);
                    audioInfo->maxSampleValue = qMax(audioInfo->maxSampleValue, out_buffer[i]);
                    audioInfo->minSampleValue = qMin(audioInfo->minSampleValue, out_buffer[i]);
                }
                audioInfo->sampleRate = frame->sample_rate;
                audioInfo->channels = frame->channels;
//                qDebug() << "frame_index:" << frame_index
//                         << "frame number of samples:" << frame->nb_samples
//                         << "avcodec_receive_frame return:" << ret
//                         << "package frame number:" << codecContext->frame_number
//                         << "packeage pts:" << packet->pts
//                         << "package size:" << packet->size;
                frame_index++;
            } while (ret >= 0);
            av_frame_free(&frame);
        }
    }
    av_free_packet(packet);
    avcodec_close(codecContext);
    avformat_close_input(&formatContext);
    av_free(out_buffer);

    audiosInfo_.insert({filePath, audioInfo});

    cleanAudiosInfo();

    return audioInfo;
}

void Audio::printAudiosInfo()
{
    qDebug() << "printAudiosInfo, there are" << audiosInfo_.size() << "audio";
    for (auto itr = audiosInfo_.begin(); itr != audiosInfo_.end(); ++itr) {
        qDebug() << "\tpath:" << itr->first << "info ref count:" << itr->second.use_count();
    }
}

void Audio::cleanAudiosInfo()
{
//    qDebug() << "Audio::cleanAudiosInfo";
//    qDebug() << "before clean:";
//    printAudiosInfo();
    for (auto itr = audiosInfo_.begin(); itr != audiosInfo_.end(); ) {
        if (itr->second.use_count() == 1) {
            itr->second.reset();
            itr = audiosInfo_.erase(itr);
        } else {
            ++itr;
        }
    }
//    qDebug() << "after clean:";
//    printAudiosInfo();
}
