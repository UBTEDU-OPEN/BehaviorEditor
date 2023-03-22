#include "yansheehts.h"

#include <QFile>
#include <QDebug>
#include <QtEndian>

YansheeHts::YansheeHts(RobotType type, QObject* parent)
    : robotType_(type)
    , QObject(parent)
    , frameDivisionCount_(10)
{
    if(type == Yanshee1_1) {
        frameByteCount_ = 33;
        servoCount_ = 17;
        reserveServoCount_ = 3;
        unitDivision_ = 20;
    } else if(type == Yanshee1_5) {
        frameByteCount_ = 40;
        servoCount_ = 22;
        reserveServoCount_ = 5;
        unitDivision_ = 20;
    }
}

bool YansheeHts::loadHtsFile(const QString& filePath, QMap<int, QMap<int,double>>& frames)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();
        if(data.size() % frameByteCount_ != 0) {
            qDebug() << "file size is invalid, size:" << data.size();
            return false;
        }
        int frameCount = data.size() / frameByteCount_;
        uint32_t totalTime = 0;
        uint32_t calcTotalTime = 0;
        qDebug() << "loadHtsFile frame count:" << frameCount;
        uint32_t uiFrameIndex = 0;
        for(int currentFrameIndex = 0; currentFrameIndex < frameCount; currentFrameIndex++) {
            QMap<int,double> servos;
            QByteArray currentFrameData = data.mid(currentFrameIndex*frameByteCount_,frameByteCount_);
            qDebug() << "current frame:" << currentFrameIndex
                     << ", frame data:" << currentFrameData.toHex();
            uint8_t* currentFrameDataPtr = reinterpret_cast<uint8_t*>(currentFrameData.data());
            if(currentFrameIndex == 0) {
                continue;
            } else if(currentFrameIndex == frameCount - 1) {
                uint8_t* totoalTimeStartPoint = currentFrameDataPtr + (frameByteCount_ - 4);
                memcpy_s(&totalTime,4,totoalTimeStartPoint,4);
                qDebug() << "total time:" << totalTime;
                continue;
            }
            uint32_t sum = 0;
            for(int i = 2; i < (frameByteCount_ - 2); i++) {
                sum += currentFrameDataPtr[i];
            }
            if(currentFrameDataPtr[0] != 0xFB
                    || currentFrameDataPtr[1] != 0xBF
                    || currentFrameDataPtr[frameByteCount_-1] != 0xED
                    || currentFrameDataPtr[frameByteCount_-2] != static_cast<uint8_t>(sum)) {
                qDebug() << "checksum or protocol flag is invalid, frame index:" << currentFrameIndex;
                return false;
            }
            int currentPos = 3; //有效数据从状态位开始
            uint8_t frameType = currentFrameDataPtr[currentPos++];
            qDebug() << "frame type:" << frameType;
            uint16_t totalFrame = 0;
            memcpy_s(&totalFrame,2,&currentFrameDataPtr[currentPos],2);
            qDebug() << "total frame count:" << totalFrame;
            currentPos += 2;
            uint16_t frameIndex = 0;
            memcpy_s(&frameIndex,2,&currentFrameDataPtr[currentPos],2);
            qDebug() << "current frame index:" << frameIndex;
            currentPos += 2;
            int lastServoPos = currentPos+servoCount_;
            for(int i = 0;currentPos < lastServoPos; currentPos++, i++) {
                uint8_t servoAngle = currentFrameDataPtr[currentPos];
                qDebug() << "servo id:" << (i+1) << ", servo angle:" << servoAngle;
                servos.insert(i+1,servoAngle);
            }
            currentPos += reserveServoCount_;
            uint8_t runTime = currentFrameDataPtr[currentPos++];
            qDebug() << "run time:" << runTime;
            uint16_t frameTotalTime = qFromBigEndian<uint16_t>(&currentFrameDataPtr[currentPos]);
            qDebug() << "frame total time" << frameTotalTime;
            if((frameTotalTime - runTime) != 1) {
                qDebug() << "#####frame" << currentFrameIndex << "is not equal 1";
            }
            calcTotalTime += frameTotalTime;
            int interval = frameTotalTime - runTime;
            uiFrameIndex += qRound((double)frameTotalTime/frameDivisionCount_);
            frames.insert(uiFrameIndex,servos);
            int intervalCount = qRound((double)interval/frameDivisionCount_);
            if(intervalCount != 0) {
                frames.insert(uiFrameIndex-intervalCount,servos);
            }
        }
        calcTotalTime *= unitDivision_;
        qDebug() << "calc total time:" << calcTotalTime;
//        qDebug() << "calc frames" << frames;
    }
    return true;
}
bool YansheeHts::saveHtsFile(const QString& filePath, const QMap<int, QMap<int,double>>& frames)
{
    QFile output(filePath);
    if(!output.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        qDebug() << filePath << "open failed." << output.errorString();
        return false;
    }
    QByteArray byteArray(frameByteCount_,0);
    output.write(byteArray);
    int lastFrameIndex = 0;
    int i = 0;
    uint32_t totalTime = 0;
    uint16_t totalFrameCount = frames.size();
    uint8_t* totalFrameCountPtr = reinterpret_cast<uint8_t*>(&totalFrameCount);
    for(auto cit = frames.cbegin(); cit != frames.cend(); cit++, i++) {
        int currentFrameIndex = cit.key();
        uint8_t runTime = static_cast<uint8_t>(currentFrameIndex-lastFrameIndex);
        runTime *= frameDivisionCount_;
        lastFrameIndex = currentFrameIndex;
        uint16_t frameTotalTime = runTime/* + 1*/; //
        totalTime += frameTotalTime;
        uint8_t frameType = 0;
        if(i == 0) {
            frameType = FrameStateType::StartFrame;
        } else if(i == frames.size() - 1) {
            frameType = FrameStateType::EndFrame;
        } else {
            frameType = FrameStateType::MiddleFrame;
        }
        uint16_t frameId = static_cast<uint16_t>(i+1);
        QByteArray currentFrameData;
        uint8_t* frameIdPtr = reinterpret_cast<uint8_t*>(&frameId);
        currentFrameData.resize(frameByteCount_);
        int currentPos = 0;
        currentFrameData[currentPos++] = static_cast<uint8_t>(0xFB);
        currentFrameData[currentPos++] = static_cast<uint8_t>(0xBF);
        currentFrameData[currentPos++] = static_cast<uint8_t>(38); //static_cast<uint8_t>(servoCount_+reserveServoCount_+8);
        currentFrameData[currentPos++] = frameType;
        currentFrameData[currentPos++] = totalFrameCountPtr[0];
        currentFrameData[currentPos++] = totalFrameCountPtr[1];
        currentFrameData[currentPos++] = frameIdPtr[0];
        currentFrameData[currentPos++] = frameIdPtr[1];
        int endPos = currentPos+servoCount_;
        auto servos = cit.value();
        for(int i = 1; currentPos < endPos && i <= servos.size(); i++) {
            currentFrameData[currentPos++] = static_cast<uint8_t>(servos[i]);
        }
        endPos = currentPos+reserveServoCount_;
        for(; currentPos < endPos;) {
            currentFrameData[currentPos++] = 0;
        }
        currentFrameData[currentPos++] = runTime;
        uint8_t* frameTotalTimePtr = reinterpret_cast<uint8_t*>(&frameTotalTime);
        currentFrameData[currentPos++] = frameTotalTimePtr[1];
        currentFrameData[currentPos++] = frameTotalTimePtr[0];
        uint32_t checksum = 0;
        for(int i = 2; i < currentPos; i++) {
            checksum += currentFrameData[i];
        }
        currentFrameData[currentPos++] = static_cast<uint8_t>(checksum);
        currentFrameData[currentPos++] = static_cast<uint8_t>(0xED);
        qDebug() << currentFrameData.size() << currentFrameData.toHex();
        output.write(currentFrameData);
    }
    totalTime *= unitDivision_;
    qDebug() << "totalTime" << totalTime;
    memcpy_s(byteArray.data()+(frameByteCount_ - 4),4,&totalTime,4);
    qDebug() << byteArray.size() << byteArray.toHex();
    output.write(byteArray);
    output.close();
    return true;
}
