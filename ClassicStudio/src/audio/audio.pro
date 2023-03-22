QT       += core

TEMPLATE = lib

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# add dll define
DEFINES += AUDIO_LIBRARY

# add dll include path
INCLUDEPATH += \
    $${TRD}/ffmpeg/$${PLATE_NAME}/include

# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET  = audiod
    } else {
        TARGET  = audio
    }
    DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    LIBS += -L$${TRD}/ffmpeg/$${PLATE_NAME}/lib \
            -lavcodec \
            -lavdevice \
            -lavfilter \
            -lavformat \
            -lavutil \
            -lpostproc \
            -lswresample \
            -lswscale
}

SOURCES += \
    audio.cpp

HEADERS += \
    audio_global.h \
    audio.h
