#include "yansheelayer.h"

#include <QFile>
#include <QByteArray>
#include <QtEndian>
#include <QDebug>

const char* YansheeLayer::kHeadFlag = "head";
const char* YansheeLayer::kHandFlag = "hand";
const char* YansheeLayer::kFootFlag = "foot";
const char* YansheeLayer::kLayerVersion = "V1.0";

YansheeLayer::YansheeLayer(QObject *parent) : QObject(parent),
    frameDivision_(200)
{

}

bool YansheeLayer::loadLayerFile(const QString& filePath,
                   QMap<int,QMap<int,double>>& headData,
                   QMap<int,QMap<int,double>>& handData,
                   QMap<int,QMap<int,double>>& footData)
{
    return true;
}

void YansheeLayer::setFrameDivision(int division)
{
    frameDivision_ = division;
}

bool YansheeLayer::saveLayerFile(const QString& filePath,
                   const QMap<int,QMap<int,double>>& headData,
                   const QMap<int,QMap<int,double>>& handData,
                   const QMap<int,QMap<int,double>>& footData)
{
    if(headData.empty() && handData.empty() && footData.empty()) {
        return false;
    }

    QFile output(filePath);
    if(!output.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        qDebug() << filePath << "open failed." << output.errorString();
        return false;
    }
    QByteArray byteArray;
    byteArray.append(kLayerVersion,4);
    byteArray.append(1,'\0');
    byteArray.append(1,'\x0');

    int lastKey = 0;
    if(!headData.empty()) {
        lastKey = headData.lastKey();
    }
    if(!handData.empty() && handData.lastKey() > lastKey) {
        lastKey = handData.lastKey();
    }
    if(!footData.empty() && footData.lastKey() > lastKey) {
        lastKey = footData.lastKey();
    }
    uint16_t frameInterval = frameDivision_;
    uint32_t totalTime = lastKey * frameInterval;
    char buf4[4] = {0};
    qToBigEndian<uint32_t>(totalTime,buf4);
    byteArray.append(buf4,4);
    byteArray.append(22,'\0');

    auto buildLayerData = [this](const QMap<int,QMap<int,double>>& source, QByteArray& dest, const char* flag){
        if(!source.empty()) {
            QByteArray framesData;
            char buf2[2] = {0};
            char buf4[4] = {0};
            uint16_t lastFrameIndex = 0;
            uint16_t currentFrameIndex = 1;
            for(auto cit = source.cbegin(); cit != source.cend(); ++cit, ++currentFrameIndex) {
                QByteArray frameData;
                QByteArray servoData;
                auto servos = cit.value();
                for(auto cit2 = servos.cbegin(); cit2 != servos.cend(); ++cit2) {
                    buf2[0] = (uint8_t)cit2.key();
                    buf2[1] = (uint8_t)cit2.value();
                    servoData.append(buf2,2);
                }
                uint16_t frameIndex = cit.key();
                uint16_t tempFrameIndex = currentFrameIndex;
                qToBigEndian<uint16_t>(tempFrameIndex,buf2);
                frameData.append(buf2,2);
                uint16_t frameSize = servoData.size() + 8;
                qToBigEndian<uint16_t>(frameSize,buf2);
                frameData.append(buf2,2);
                memset(buf2,0,2);
//                buf2[0]=0x3;
//                buf2[1]=0xE8; //等待1000ms,大端
                frameData.append(buf2,2);
                buf2[0] = (frameIndex - lastFrameIndex)*frameDivision_/20;
                buf2[1] = 0; //0线性 1曲线
                frameData.append(buf2,2);
                frameData.append(servoData);
                framesData.append(frameData);
                lastFrameIndex = frameIndex;
            }
            dest.append(flag,4);
            dest.append(1,'\0');
            uint32_t framesDataSize = framesData.size() + 15;
            qToBigEndian<uint32_t>(framesDataSize,buf4);
            dest.append(buf4,4);
            uint32_t layerTotalTime = source.lastKey()*frameDivision_;
            qToBigEndian<uint32_t>(layerTotalTime,buf4);
            dest.append(buf4,4);
            uint16_t frameCount = source.size();
            qToBigEndian<uint16_t>(frameCount,buf2);
            dest.append(buf2,2);
            dest.append(framesData);
            qDebug() << framesData.size() << dest.size();

        }
    };
    QByteArray headLayerData;
    buildLayerData(headData,headLayerData,kHeadFlag);
    byteArray.append(headLayerData);
    QByteArray handLayerData;
    buildLayerData(handData,handLayerData,kHandFlag);
    byteArray.append(handLayerData);
    QByteArray footLayerData;
    buildLayerData(footData,footLayerData,kFootFlag);
    byteArray.append(footLayerData);
    byteArray.append(32,'\0');

    output.write(byteArray);
    output.close();
    return true;
}
