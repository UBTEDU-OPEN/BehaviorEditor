#ifndef MD5_H
#define MD5_H

#include "utilsGlobal.h"

#include <QString>
#include <QObject>
#include <QThread>

class UTILS_EXPORT MD5  : public QThread
{
    Q_OBJECT

public:
    static QString strMd5(const QString &str);
    static QString fileMd5(const QString &file);

public:
    explicit MD5(const QString &md5Str, const QString &filePath, QObject *parent);
    bool isValid() const { return valid_; }

    void run() override;

private:
    QString md5Str_;
    QString filePath_;

    bool valid_;
};

#endif // MD5_H
