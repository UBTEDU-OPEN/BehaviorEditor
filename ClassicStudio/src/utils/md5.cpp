#include "md5.h"

#include "logHelper.h"

#include <QByteArray>
#include <QCryptographicHash>
#include <QFile>

QString MD5::strMd5(const QString &str)
{
    QString md5;
    QByteArray input, output;
    input.append(str);
    QCryptographicHash md(QCryptographicHash::Md5);
    md.addData(input);
    output = md.result();
    md5.append(output.toHex());
    return md5;
}

QString MD5::fileMd5(const QString &file)
{
    QFile localFile(file);

    if (!localFile.open(QFile::ReadOnly)) {
        LOG(INFO) << "file open error.";
        return "";
    }

    QCryptographicHash ch(QCryptographicHash::Md5);

    quint64 totalBytes = 0;
    quint64 bytesWritten = 0;
    quint64 bytesToWrite = 0;
    quint64 loadSize = 1024 * 10;
    QByteArray buf;

    totalBytes = localFile.size();
    bytesToWrite = totalBytes;

    while (1) {
        if (bytesToWrite > 0) {
            buf = localFile.read(qMin(bytesToWrite, loadSize));
            ch.addData(buf);
            bytesWritten += buf.length();
            bytesToWrite -= buf.length();
            buf.resize(0);
        } else {
            break;
        }

        if (bytesWritten == totalBytes) {
            break;
        }
    }
    localFile.close();
    QByteArray md5 = ch.result();
    QString md5Str;
    md5Str.append(md5.toHex());

    return md5Str;
}

MD5::MD5(const QString &md5Str, const QString &filePath, QObject *parent)
    : md5Str_(md5Str)
    , filePath_(filePath)
    , valid_(false)
    , QThread(parent)
{}

void MD5::run()
{
    QString fileMD5 = MD5::fileMd5(filePath_);
    LOG(INFO) << "md5:" << md5Str_.toStdString() << " file md5:" << fileMD5.toStdString();
    valid_ = (fileMD5 == md5Str_);
}
