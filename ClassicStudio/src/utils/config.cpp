#include "config.h"
#include "logHelper.h"
#include "fileDirHandler.h"

#include <QSettings>
#include <QDir>
#include <QStringList>
#include <QByteArray>

const QString Config::CONFIG_FOLDER = "configs";
const QString Config::CONFIG_FILE = "config.ini";

const QString Config::LOG_SECTION = "log";
const QString Config::EDITOR_SECTION = "editor";

const QString Config::LOG_LEVEL_KEY = "log_level";
const QString Config::LOG_BUF_SECS_KEY = "log_buf_secs";
const QString Config::LOG_MAX_SIZE_KEY = "log_max_size"; //MB
const QString Config::ROBOT_SN_KEY = "robot_sn";

const int Config::LOG_LEVEL_DEFAULT_VALUE = 0; // 0 - info; 1 - warning; 2 - error; 3 - fatal
const int Config::LOG_BUF_SECS_DEFAULT_VALUE = 30;
const int Config::LOG_MAX_SIZE_DEFAULT_VALUE = 50;

QString Config::configFileAbsPath()
{
    QString configFilePath = CONFIG_FOLDER + "/" + CONFIG_FILE;
    configFilePath = FileDirHandler::absolutePath(configFilePath);
    return configFilePath;
}

int Config::getLogLevel()
{
    QSettings settings(configFileAbsPath(), QSettings::IniFormat);
    settings.beginGroup(LOG_SECTION);
    int level = settings.value(LOG_LEVEL_KEY, LOG_LEVEL_DEFAULT_VALUE).toInt();
    settings.endGroup();
    return level;
}

int Config::getLogBufSecs()
{
    QSettings settings(configFileAbsPath(), QSettings::IniFormat);
    settings.beginGroup(LOG_SECTION);
    int bufSecs = settings.value(LOG_BUF_SECS_KEY, LOG_BUF_SECS_DEFAULT_VALUE).toInt();
    settings.endGroup();
    return bufSecs;
}

int Config::getLogMaxSize()
{
    QSettings settings(configFileAbsPath(), QSettings::IniFormat);
    settings.beginGroup(LOG_SECTION);
    int maxSize = settings.value(LOG_MAX_SIZE_KEY, LOG_MAX_SIZE_DEFAULT_VALUE).toInt();
    settings.endGroup();
    return maxSize;
}

void Config::setRobotSN(QString sn)
{
    QSettings settings(configFileAbsPath(), QSettings::IniFormat);
    settings.beginGroup(EDITOR_SECTION);
    settings.setValue(ROBOT_SN_KEY, sn);
    settings.endGroup();
    settings.sync();
}

QString Config::getRobotSN()
{
    QSettings settings(configFileAbsPath(), QSettings::IniFormat);
    settings.beginGroup(EDITOR_SECTION);
    QString sn = settings.value(ROBOT_SN_KEY, "").toString();
    settings.endGroup();
    return sn;
}


