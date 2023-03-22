QT -= gui

TEMPLATE = lib
DEFINES += HTS_LIBRARY

CONFIG += c++11

# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET   = htsd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    } else {
        TARGET   = hts
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
}

SOURCES += \
    yansheehts.cpp \
    yansheelayer.cpp

HEADERS += \
    hts_global.h \
    yansheehts.h \
    yansheelayer.h
