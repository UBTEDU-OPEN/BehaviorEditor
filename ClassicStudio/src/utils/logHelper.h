#ifndef LOGHELPER_H
#define LOGHELPER_H

#include "utilsGlobal.h"

#include "glog/logging.h"

class UTILS_EXPORT LogHelper
{
public:
    ~LogHelper();
    static void init(const char* argv);

private:
    LogHelper(const char* argv);
};

#endif // LOGHELPER_H
