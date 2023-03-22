# add common infomation
QT       += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = lib

# add dll define
DEFINES += QT_DEPRECATED_WARNINGS \
           ROBOTVIEW_LIBRARY

equals(ROBOT_NAME, "YANSHEE1_5") {
   message(robotview BUILD_YANSHEE15)
   DEFINES += YANSHEE1_5
} else {
   message(robotview BUILD_YANSHEE11)
   DEFINES += YANSHEE1_1
}

# add dll include path
INCLUDEPATH += $${SRC}/fbx \
               $${SRC}/robotcfg\
               $${SRC}/datacenter \
               $${SRC}/utils \
               $${TRD}/glog/include

# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET  = robotviewd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lfbxd \
                -lrobotcfgd\
                -ldatacenterd \
                -lutilsd
    } else {
        TARGET  = robotview
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lfbx \
                -lrobotcfg\
                -ldatacenter \
                -lutils
    }
}

# add source/header/form/resource file
SOURCES += robotview.cpp

HEADERS += robotview_global.h \
           robotview.h
