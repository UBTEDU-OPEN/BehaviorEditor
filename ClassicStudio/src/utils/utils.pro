# add common infomation
QT += core
TEMPLATE = lib

# add dll define
DEFINES += UTILS_LIB

# add dll include path
INCLUDEPATH += \
    $${TRD}/glog/include


# add source/header/form/resource file
SOURCES += \
    cmd5.cpp \
    config.cpp \
    fileDirHandler.cpp \
    logHelper.cpp \
    md5.cpp \

HEADERS += utilsGlobal.h \
    cmd5.h \
    config.h \
    fileDirHandler.h \
    logHelper.h \
    md5.h

# windows
win32 {
    CONFIG(debug, debug|release) {
        TARGET = utilsd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${TRD}/glog/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglogd

    } else {
        TARGET = utils
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${TRD}/glog/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglog
    }
}


