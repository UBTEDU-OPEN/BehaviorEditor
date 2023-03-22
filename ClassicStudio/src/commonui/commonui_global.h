#ifndef COMMONUI_GLOBAL_H
#define COMMONUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMMONUI_LIBRARY)
#  define COMMONUI_EXPORT Q_DECL_EXPORT
#else
#  define COMMONUI_EXPORT Q_DECL_IMPORT
#endif

#endif // COMMONUI_GLOBAL_H
