#ifndef DATACENTER_GLOBAL_H
#define DATACENTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DATACENTER_LIBRARY)
#  define DATACENTER_EXPORT Q_DECL_EXPORT
#else
#  define DATACENTER_EXPORT Q_DECL_IMPORT
#endif

#endif // DATACENTER_GLOBAL_H