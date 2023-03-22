# add common infomation
QT      += core gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = lib

# add dll define
DEFINES += QT_DEPRECATED_WARNINGS \
           BEHAVIOREDIT_LIBRARY

# add dll include path
INCLUDEPATH += \
    $${SRC}/audio \
    $${SRC}/robotcfg \
    $${SRC}/behavior \
    $${SRC}/common \
    $${SRC}/commonui \
    $${SRC}/datacenter \
    $${SRC}/utils \
    $${TRD}/glog/include

# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET  = behavioreditd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS   += -L$${DESTDIR} \
                  -laudiod \
                  -lrobotcfgd \
                  -lcommonuid \
                  -ldatacenterd \
                  -lutilsd
        LIBS += -L$${TRD}/glog/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglogd
    } else {
        TARGET   = behavioredit
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS   += -L$${DESTDIR} \
                  -laudio \
                  -lrobotcfg \
                  -lcommonui \
                  -ldatacenter \
                  -lutils
        LIBS += -L$${TRD}/glog/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglog
    }
}

# add source/header/form/resource file
SOURCES += \
    actionframewidget.cpp \
    actiongroupwidget.cpp \
    audioitem.cpp \
    axis.cpp \
    behavioredit.cpp \
    behaviorlayercontainer.cpp \
    controlwidget.cpp \
    distancewidget.cpp \
    frameintervaleditor.cpp \
    frameoverline.cpp \
    framepointer.cpp \
    framepointertop.cpp \
    frameratesettingpanel.cpp \
    frameratesettingswidget.cpp \
    frameviewer.cpp \
    frameviewercontainer.cpp \
    frameviewplaceholder.cpp \
    labelblock.cpp \
    labelcontenteditor.cpp \
    layerbgwidget.cpp \
    musicwidget.cpp \
    servoitem.cpp \
    servotreeitemdelegate.cpp

HEADERS += \
    actionframewidget.h \
    actiongroupwidget.h \
    audioitem.h \
    axis.h \
    behavioredit.h \
    behavioreditglobal.h \
    behaviorlayercontainer.h \
    commondefine.h \
    controlwidget.h \
    distancewidget.h \
    frameintervaleditor.h \
    frameoverline.h \
    framepointer.h \
    framepointertop.h \
    frameratesettingpanel.h \
    frameratesettingswidget.h \
    frameviewer.h \
    frameviewercontainer.h \
    frameviewplaceholder.h \
    labelblock.h \
    labelcontenteditor.h \
    layerbgwidget.h \
    musicwidget.h \
    servoitem.h \
    servotreeitemdelegate.h

FORMS += \
    actionframewidget.ui \
    actiongroupwidget.ui \
    controlwidget.ui \
    distancewidget.ui \
    frameintervaleditor.ui \
    framepointertop.ui \
    frameratesettingpanel.ui \
    frameratesettingswidget.ui \
    frameviewplaceholder.ui \
    labelcontenteditor.ui \
    musicwidget.ui

RESOURCES += \
    behavioredit.qrc
