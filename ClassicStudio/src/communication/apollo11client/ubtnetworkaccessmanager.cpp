#include "ubtnetworkaccessmanager.h"
#include<QBuffer>
#include<QIODevice>
#include<QBuffer>

UbtNetworkAccessManager::UbtNetworkAccessManager(QObject *parent)
    :QNetworkAccessManager(parent)
{

}

QNetworkReply *UbtNetworkAccessManager::deleteResource(const QNetworkRequest &request, const QByteArray &data)
{


    QBuffer *buffer = new QBuffer;
    buffer->setData(data);
    buffer->open(QIODevice::ReadOnly);

    QNetworkReply *reply = createRequest(QNetworkAccessManager::DeleteOperation, request,buffer);
//    buffer->setParent(reply);

    return reply;
}
