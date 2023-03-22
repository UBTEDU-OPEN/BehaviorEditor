#ifndef GLOBAL_H
#define GLOBAL_H

#if defined(ANDROID_NDK) || defined(__BORLANDC__) || defined(__QNXNTO__)
#   include <ctype.h>
#   include <limits.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#	if defined(__PS3__)
#		include <stddef.h>
#	endif
#else
#   include <cctype>
#   include <climits>
#   include <cstdio>
#   include <cstdlib>
#   include <cstring>
#endif

#ifdef _WIN32
#   ifdef FBX_EXPORT
#       define FBX_LIB __declspec(dllexport)
#   elif defined(FBX_IMPORT)
#       define FBX_LIB __declspec(dllimport)
#   else
#       define FBX_LIB
#   endif
#elif __GNUC__ >= 4
#   define FBX_LIB __attribute__((visibility("default")))
#else
#   define FBX_LIB
#endif

#endif // GLOBAL_H
