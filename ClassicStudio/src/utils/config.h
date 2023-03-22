#ifndef CONFIG_H
#define CONFIG_H

#include "utilsGlobal.h"

#include <QString>

class UTILS_EXPORT Config
{
public:
    // folder
    static const QString CONFIG_FOLDER;
    // file
    static const QString CONFIG_FILE;
    // section
    static const QString LOG_SECTION;
    static const QString EDITOR_SECTION;
    // key
    static const QString LOG_LEVEL_KEY;
    static const QString LOG_BUF_SECS_KEY;
    static const QString LOG_MAX_SIZE_KEY;
    static const QString ROBOT_SN_KEY;
    static const QString NO_SET_TIME_TIPS_KEY;
    static const QString NO_MORE_TIPS_KEY;
    // value
    static const int LOG_LEVEL_DEFAULT_VALUE;
    static const int LOG_BUF_SECS_DEFAULT_VALUE;
    static const int LOG_MAX_SIZE_DEFAULT_VALUE;

    static QString configFileAbsPath();
    static int getLogLevel();
    static int getLogBufSecs();
    static int getLogMaxSize();

    static void setRobotSN(QString sn);
    static QString getRobotSN();
};

#endif // CONFIG_H
