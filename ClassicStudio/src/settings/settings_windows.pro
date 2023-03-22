#The following code works on Windows (at least with all the recent MSVC compilers - didn't test MinGW), Mac OS X (clang) and Linux (GCC).
#Feel free to omit the first clause and refer to QT_ARCH directly if you don't need Qt 4 support.
greaterThan(QT_MAJOR_VERSION, 4) {
    TARGET_ARCH=$${QT_ARCH}
} else {
    TARGET_ARCH=$${QMAKE_HOST.arch}
}

contains(TARGET_ARCH, x86_64) {
    message("64 bit")
    PLATE_NAME = win64
} else {
    message("32 bit")
    PLATE_NAME = win32
}

CONFIG(release, debug|release) {
    message("release")
    DIR_NAME = release
} else {
    message("debug")
    DIR_NAME = debug
}

BASE    = $$PWD/../..
BIN     = $${BASE}/bin
SRC     = $${BASE}/src
TRD     = $${SRC}/trd
ROBOT_NAME = YANSHEE1_1

cache(DIR_NAME, set, DIR_NAME)
cache(PLATE_NAME, set, PLATE_NAME)
cache(BASE, set, BASE)
cache(BIN, set, BIN)
cache(SRC, set, SRC)
cache(TRD, set, TRD)
cache(ROBOT_NAME, set, ROBOT_NAME)

export (DIR_NAME)
export (PLATE_NAME)
export (BASE)
export (SRC)
export (TRD)
export (ROBOT_NAME)

