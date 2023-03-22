#ifndef CLASSSTUDIOSETTINGS_H
#define CLASSSTUDIOSETTINGS_H

#include<QSettings>
#include "robotcfg_global.h"

class ROBOTCFGSHARED_EXPORT ClassStudioSettings
{
public:
    ClassStudioSettings();
    ~ClassStudioSettings();
    bool getNoMoreTips();
    void setNoMoreTips(bool isNoMoreTips);
    bool getSetTimeTips();
    void setSetTimeTips(bool noSetTimeTips);
    void setConnectedRobotSN(QString sn);
    QString getConnectedRobotSN();
private:
    QSettings *m_psetting ;
};

#endif // CLASSSTUDIOSETTINGS_H
