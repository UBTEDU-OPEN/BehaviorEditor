﻿#ifndef BEHAVIOREDITGLOBAL_H
#define BEHAVIOREDITGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BEHAVIOREDIT_LIBRARY)
#  define BEHAVIOREDIT_EXPORT Q_DECL_EXPORT
#else
#  define BEHAVIOREDIT_EXPORT Q_DECL_IMPORT
#endif

#endif // BEHAVIOREDITGLOBAL_H
