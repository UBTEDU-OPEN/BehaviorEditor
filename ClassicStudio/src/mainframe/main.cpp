#include "mainwindow.h"
#include <QApplication>
#include <robotcfg.h>
#include <QMutex>
#include <QFile>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QScreen>

#include "HttpHandler.h"
#include "httpserver.h"
#include "HttpConnection.h"
#include "predefinedposture.h"

#include "datacenter.h"
#include<QTranslator>
#include "classstudiosettings.h"
#include "application.h"
#include "logHelper.h"

using namespace Pillow;
using namespace std;
class HttpHandlerStats : public HttpHandler
{
public:
    HttpHandlerStats(QObject* parent = 0) : HttpHandler(parent) {}
    virtual bool handleRequest(Pillow::HttpConnection* rq) {
        HttpServer* server = NULL;
        HttpHandler* handler = NULL;

        for (QObject* h = parent(); h != NULL; h = h->parent()) {
            if (qobject_cast<HttpHandler*>(h)) handler = static_cast<HttpHandler*>(h);
            else if (qobject_cast<HttpServer*>(h)) server = static_cast<HttpServer*>(h);
        }

        if (rq->requestPath() == "/_stats") {
            QByteArray result;
            if (server != NULL) {
                result.append("Alive connections: ").append(QByteArray::number(server->findChildren<Pillow::HttpConnection*>().size())).append("\n");
            }
            if (handler != NULL) {
                result.append("Alive big file transfers: ").append(QByteArray::number(handler->findChildren<Pillow::HttpHandlerFileTransfer*>().size())).append("\n");
            }

            rq->writeResponse(200, HttpHeaderCollection(), result);
            return true;
        }
        return false;
    }
};

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

    static QMutex mutex;
    mutex.lock();
    QString text;
    bool flag = false;
    switch(type) {
    case QtDebugMsg://qDebug
        flag = true;
        text = QString(":");
        break;
    case QtInfoMsg://qInfo
        text = QString(":");
        break;
    case QtWarningMsg://qWarning
        text = QString(":");
        break;
    case QtCriticalMsg://qCritical
        text = QString(":");
        break;
    case QtFatalMsg://qFatal
        text = QString(":");
        abort();
    }

    if (flag) {
#ifdef QT_NO_DEBUG
        cout << msg.toStdString() << endl;
#else
        cout << msg.toStdString() << "," << context.file << "," << context.line << "," << context.function << endl;
#endif
    }

    QDateTime dt = QDateTime::currentDateTime();
    QString message = QString("[%1] %2 %3")
            .arg(dt.toString("yyyy-MM-dd hh:mm:ss ddd"))
            .arg(text)
            .arg(msg);
    QString strBasePath = QCoreApplication::applicationDirPath() + "/log";
    QDir dir(strBasePath);
    if ( !dir.exists() ) {
        dir.mkdir(strBasePath);
    }
    QString logpath = strBasePath + QString("/log[%1].txt").arg(dt.toString("yyyy-MM-dd"));
    QFile file(logpath);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << endl;
    file.flush();
    file.close();
    mutex.unlock();
}

int main(int argc, char *argv[])
{
//    qInstallMessageHandler(myMessageOutput);
    /**< 设置属于不同窗口的QOpenGLWidget实例之间的共享 */
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
    QApplication a(argc, argv);
    QFile styleSheet(":/qss/res/qss/mainwindow.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        a.setStyleSheet(styleSheet.readAll());
    }
    QString strRunDir = qApp->applicationDirPath();
#ifdef YANSHEE1_5
    QString strRobotCfgPath = strRunDir + "/resource/robot/yanshee1_5.xml";
#else
    QString strRobotCfgPath = strRunDir + "/resource/robot/yanshee1_1.xml";
#endif
    RobotConfig->load(strRobotCfgPath);
#if 0
    auto postureInstance = PredefinedPosture::getInstance();
    QString predefinedPostureFilePath = strRunDir + "/resource/robot/classic_posture.xml";
    postureInstance->loadFile(predefinedPostureFilePath);
#endif

    DataCenter::getInstance()->init(); //创建实例并初始化

    // Set application information
    a.setApplicationName (QObject::tr("Yanshee Studio"));
    a.setApplicationVersion ("1.0.0");

    //初始化日志记录模块
    LogHelper::init(argv[0]);

#if 0
    HttpServer server(QHostAddress(QHostAddress::LocalHost), 9876);
    if (!server.isListening()) {
        return 0;
    }

    QString ucodeStr = strRunDir + "/resource/lottie_web/";
    HttpHandlerStack* handler = new HttpHandlerStack(&server);
    HttpHandlerLog handleLog(handler);
    HttpHandlerStats handleStats(handler);
    HttpHandlerFile handleFile(ucodeStr, handler);
    HttpHandler404 handle404(handler);
    Q_UNUSED(handleLog)
    Q_UNUSED(handleStats)
    Q_UNUSED(handleFile)
    Q_UNUSED(handle404)
    QObject::connect(&server, SIGNAL(requestReady(Pillow::HttpConnection*)),
                     handler, SLOT(handleRequest(Pillow::HttpConnection*)));
#endif
    QTranslator translator;
    if (translator.load("yansheestudio")) {
        a.installTranslator(&translator);
    }
    MainWindow w;
//    QObject::connect(&a,&Application::ctrlKeyEvent,&w,&MainWindow::onCtrlKeyEvent);
//    QObject::connect(&a,&Application::normalKeyEvent,&w,&MainWindow::onNormalKeyEvent);
    w.show();
    int nRet = a.exec();
#if 0
    handler->deleteLater();
#endif
    return nRet;
}
