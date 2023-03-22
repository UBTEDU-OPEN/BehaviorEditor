#include "fileDirHandler.h"

#include <QDir>


const QString FileDirHandler::DIR_SPLIT_CHAR = QDir::separator();

QString FileDirHandler::absolutePath(const QString &path, const QString &relative)
{
    QDir dir(relative);
    return dir.absoluteFilePath(path);
}

