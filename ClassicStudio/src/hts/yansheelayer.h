#ifndef YANSHEELAYER_H
#define YANSHEELAYER_H

#include <QObject>
#include <QMap>

#include "hts_global.h"

class HTS_EXPORT YansheeLayer : public QObject
{
    Q_OBJECT
public:
    explicit YansheeLayer(QObject *parent = nullptr);
    void setFrameDivision(int division);
    bool loadLayerFile(const QString& filePath,
                       QMap<int,QMap<int,double>>& headData,
                       QMap<int,QMap<int,double>>& handData,
                       QMap<int,QMap<int,double>>& footData);

    bool saveLayerFile(const QString& filePath,
                       const QMap<int,QMap<int,double>>& headData,
                       const QMap<int,QMap<int,double>>& handData,
                       const QMap<int,QMap<int,double>>& footData);

private:
    static const int kInfoLength = 32;
    static const char* kHeadFlag;
    static const char* kHandFlag;
    static const char* kFootFlag;
    static const char* kLayerVersion;

    int frameDivision_;
};

#endif // YANSHEELAYER_H
