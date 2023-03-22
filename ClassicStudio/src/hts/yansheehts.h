#ifndef YANSHEEHTS_H
#define YANSHEEHTS_H

#include "hts_global.h"
#include <QObject>

class HTS_EXPORT YansheeHts : QObject
{
public:
    enum RobotType {
        Yanshee1_1 = 1,
        Yanshee1_5 = 2
    };
    explicit YansheeHts(RobotType type, QObject* parent = nullptr);
    enum FrameStateType {
        StartFrame = 0x01,
        MiddleFrame = 0x02,
        EndFrame = 0x03
    };
    bool loadHtsFile(const QString& filePath, QMap<int, QMap<int,double>>& frames);
    bool saveHtsFile(const QString& filePath, const QMap<int, QMap<int,double>>& frames);
    void setDivisionCount(int divisionCount) { frameDivisionCount_ = divisionCount; }
    int divisionCount() { return frameDivisionCount_; }
    int frameRate() { return 1000 / (unitDivision_*frameDivisionCount_); }
    int unitDivision() { return unitDivision_; }

private:
    RobotType robotType_;
    int servoCount_;
    int reserveServoCount_;
    int frameByteCount_;
    int unitDivision_;
    int frameDivisionCount_;
};

#endif // YANSHEEHTS_H
