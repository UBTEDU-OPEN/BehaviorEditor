#ifndef FILEDIRHANDLER_H
#define FILEDIRHANDLER_H

#include "utilsGlobal.h"

#include <QCoreApplication>
#include <QString>

class UTILS_EXPORT FileDirHandler
{
public:
    static const QString DIR_SPLIT_CHAR;

public:
    FileDirHandler() = delete;

    static QString absolutePath(const QString &path, const QString &relative = QCoreApplication::applicationDirPath());
};

#endif // FILEDIRHANDLER_H
