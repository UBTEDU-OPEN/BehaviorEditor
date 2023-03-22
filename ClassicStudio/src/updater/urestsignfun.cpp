#include "urestsignfun.h"
#include "ubtsign.h"
#include <QTimer>
#include <QNetworkRequest>
#include <QDateTime>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QUrl>

static UBTSign gUbtSign;

void URestSignFun::addSignRequest (QNetworkRequest *request,
                            const QString& strUrl,
                            const QString& strAppId,
                            const QString& strAppKey,
                            const QString& strSnNum )
{
    const std::string snNum = strSnNum.toStdString();
    QString strUbtSign = QString::fromStdString(gUbtSign.getURestSign(snNum));

    request->setUrl(QUrl(strUrl));
    request->setRawHeader(QByteArray("Content-Type"), QByteArray("application/json"));
    request->setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    request->setRawHeader(QByteArray("X-UBT-AppId"), strAppId.toUtf8());
    request->setRawHeader(QByteArray("X-UBT-Sign"), strUbtSign.toUtf8());
    request->setRawHeader(QByteArray("X-UBT-DeviceId"), strSnNum.toUtf8());
    request->setRawHeader(QByteArray("Access-Version"), QByteArray("v2"));
    request->setRawHeader(QByteArray("common-info"), QByteArray("{}"));
    request->setRawHeader(QByteArray("device-info"), QByteArray("{}"));
}

int URestSignFun::waitForReply(QNetworkReply *reply)
{
    QTimer timer;
    timer.setInterval(REQUEST_URL_TIME_OUT);
    timer.setSingleShot(true);

    /**
     * @brief 分三种情况退出事件循环(任意一个条件触发则退出)
     *        1、计时器超时（30秒）
     *        2、后台回复数据
     *        3、与后台通讯出错
     */
    QEventLoop loop;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    timer.start();
    loop.exec();  // 启动事件循环

    int nState = EN_REQUEST_WAIT_UNKOWN;
    if ( timer.isActive() ) {
        timer.stop();
        if ( reply->error() != QNetworkReply::NoError ){
            nState = EN_REQUEST_WAIT_NETERROR;
        }else{
            nState = EN_REQUEST_WAIT_RETURN;
        }
    } else {
        QObject::disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        nState = EN_REQUEST_WAIT_TIME_OUT;
    }

    return nState;
}
