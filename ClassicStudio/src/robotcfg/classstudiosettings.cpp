#include "classstudiosettings.h"
#include<QCoreApplication>

ClassStudioSettings::ClassStudioSettings()
{
    m_psetting = new QSettings(QCoreApplication::applicationDirPath() + "/Config.ini", QSettings::IniFormat);
}

ClassStudioSettings::~ClassStudioSettings()
{
    delete m_psetting;
}

bool ClassStudioSettings::getNoMoreTips()
{
    return m_psetting->value("noMoreTips",false).toBool();
}

void ClassStudioSettings::setNoMoreTips(bool isNoMoreTips)
{
    m_psetting->setValue("noMoreTips",isNoMoreTips);

}

bool ClassStudioSettings::getSetTimeTips()
{
    return m_psetting->value("noSetTimeTips",false).toBool();
}

void ClassStudioSettings::setSetTimeTips(bool noSetTimeTips)
{
    m_psetting->setValue("noSetTimeTips",noSetTimeTips);
}

void ClassStudioSettings::setConnectedRobotSN(QString sn)
{
    QVariant v(sn);
    m_psetting->setValue("connectedSN",v);
}

QString ClassStudioSettings::getConnectedRobotSN()
{
    QVariant v = m_psetting->value("connectedSN");
    return v.toString();
}
