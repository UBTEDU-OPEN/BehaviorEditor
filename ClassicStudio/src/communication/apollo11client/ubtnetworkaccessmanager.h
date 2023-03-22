#ifndef UBTNETWORKACCESSMANAGER_H
#define UBTNETWORKACCESSMANAGER_H

#include"qnetworkaccessmanager.h"

class  UbtNetworkAccessManager : public QNetworkAccessManager
{
public:
    UbtNetworkAccessManager(QObject *parent);
    QNetworkReply *deleteResource(const QNetworkRequest &request, const QByteArray &data);
};

#endif // UBTNETWORKACCESSMANAGER_H
