#-------------------------------------------------
#
# Project created by QtCreator 2019-10-17T17:02:04
#
#-------------------------------------------------
TEMPLATE   = lib
QT        += network
DEFINES   += UPDATER_LIBRARY \
             QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/QSimpleUpdater/include \
               $${TRD}/quazip/include \
               $${TRD}/zlib/include

win32{
    CONFIG(release, debug|release){
        TARGET  = updater
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS    += -L$${TRD}/zlib/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lzlibwapi \
                   -L$${TRD}/quazip/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lquazip \
                   -L$${TRD}/ubtsign/lib/$${PLATE_NAME}/ \
                   -lubtsign \
    }else{
        TARGET  = updaterd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS    += -L$${TRD}/zlib/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lzlibwapid \
                   -L$${TRD}/quazip/lib/$${PLATE_NAME}/$${DIR_NAME} \
                   -lquazipd
    }
}

include ($$PWD/QSimpleUpdater/QSimpleUpdater.pri)

SOURCES += $$PWD/updaterwindow.cpp \
           $$PWD/urestsignfun.cpp

HEADERS += $$PWD/updater_global.h \
           $$PWD/updaterwindow.h \
           $$PWD/urestsignfun.h \
           $$PWD/uhttpdefine.h \
           $$PWD/ubtsign.h

FORMS   += $$PWD/updaterwindow.ui

RESOURCES += \
           $$PWD/styles.qrc \
           $$PWD/fonts.qrc

