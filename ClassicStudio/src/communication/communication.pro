QT -= gui
QT += core network

TEMPLATE = lib
DEFINES += COMMUNICATION_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# windows plate
win32{
    INCLUDEPATH += $${SRC}/robotcfg \
                   $${SRC}/utils \
                   $${TRD}/glog/include
    CONFIG(debug, debug|release){
        TARGET   = communicationd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS   += -L$${DESTDIR} \
                  -lrobotcfgd \
                  -lutilsd
        LIBS += -L$${TRD}/glog/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglogd
    } else {
        TARGET   = communication
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS   += -L$${DESTDIR} \
                  -lrobotcfg \
                  -lutils
        LIBS += -L$${TRD}/glog/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglog
    }

    equals(ROBOT_NAME, "YANSHEE1_5") {
        message(BUILD_YANSHEE15)
        DEFINES += YANSHEE1_5
        include(apollo15client/client.pri)
    } else {
        message(BUILD_YANSHEE11)
        DEFINES += YANSHEE1_1
        include(apollo11client/client.pri)
    }
}

SOURCES += \
    apollocommunication.cpp

HEADERS += \
    apollocommunication.h \
    communication_global.h
