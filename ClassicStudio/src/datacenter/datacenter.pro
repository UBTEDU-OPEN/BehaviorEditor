QT += core gui widgets network

TEMPLATE = lib

CONFIG += c++11

# add dll define
DEFINES += QT_DEPRECATED_WARNINGS \
           DATACENTER_LIBRARY

# add dll include path
INCLUDEPATH += $${SRC}/robotcfg \
               $${SRC}/behavior \
               $${SRC}/common \
               $${SRC}/communication \
               $${SRC}/commonui \
               $${SRC}/robotsearch \
               $${SRC}/hts \
               $${SRC}/utils \
               $${TRD}/glog/include

equals(ROBOT_NAME, "YANSHEE1_5") {
   message(robotview BUILD_YANSHEE15)
   DEFINES += YANSHEE1_5
} else {
   message(robotview BUILD_YANSHEE11)
   DEFINES += YANSHEE1_1
}

# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET  = datacenterd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS   += -L$${DESTDIR} \
                  -lbehaviord \
                  -lcommunicationd \
                  -lcommonuid \
                  -lrobotsearchd \
                  -lhtsd \
                  -lrobotcfgd \
                  -lutilsd
        LIBS += -L$${TRD}/glog/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglogd
    } else {
        TARGET   = datacenter
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS   += -L$${DESTDIR} \
                  -lbehavior \
                  -lcommunication \
                  -lcommonui \
                  -lrobotsearch \
                  -lhts \
                  -lrobotcfg \
                  -lutils
        LIBS += -L$${TRD}/glog/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglog
    }
}


SOURCES += \
    actiongroup.cpp \
    datacenter.cpp

HEADERS += \
    actiongroup.h \
    datacenter_global.h \
    datacenter.h
