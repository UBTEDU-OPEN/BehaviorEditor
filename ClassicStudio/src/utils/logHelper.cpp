#include "logHelper.h"
#include "config.h"

#include <QDir>
#include <QCoreApplication>

const QString kLogDir("log");
const char* kRelativePath = "./log/";

LogHelper::LogHelper(const char* argv)
{
    QDir dir(QCoreApplication::applicationDirPath());
    if(!dir.exists(kLogDir)) {
        dir.mkdir(kLogDir);
    }
    google::InitGoogleLogging(argv);
    auto logLevel = Config::getLogLevel();
    google::SetLogDestination(logLevel, kRelativePath);
    FLAGS_logbufsecs = Config::getLogBufSecs();
    FLAGS_max_log_size = Config::getLogMaxSize();
    FLAGS_alsologtostderr = 1;
    FLAGS_stop_logging_if_full_disk = true;
}

LogHelper::~LogHelper()
{
    google::ShutdownGoogleLogging();
}

void LogHelper::init(const char* argv)
{
    static LogHelper inst(argv);
}


