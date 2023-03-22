#ifndef HTS_GLOBAL_H
#define HTS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HTS_LIBRARY)
#  define HTS_EXPORT Q_DECL_EXPORT
#else
#  define HTS_EXPORT Q_DECL_IMPORT
#endif

#endif // HTS_GLOBAL_H
