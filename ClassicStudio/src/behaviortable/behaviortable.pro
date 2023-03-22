QT += widgets webenginewidgets

TEMPLATE = lib
DEFINES += BEHAVIORTABLE_LIBRARY

CONFIG += c++11

INCLUDEPATH += $${SRC}/robotcfg \
               $${SRC}/common \
               $${SRC}/commonui \
               $${SRC}/datacenter

# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET  = behaviortabled
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lrobotcfgd \
                -lcommonuid \
                -ldatacenterd
    } else {
        TARGET   = behaviortable
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS   += -L$${DESTDIR} \
                  -lrobotcfg \
                  -lcommonui \
                  -ldatacenter
    }
}

SOURCES += \
    behaviorcurvewidget.cpp \
    behaviortable.cpp \
    behaviortabledelegate.cpp \
    behaviortablewidget.cpp \
    horizontalheaderview.cpp \
    tipwidget.cpp

HEADERS += \
    behaviorcurvewidget.h \
    behaviortable_global.h \
    behaviortable.h \
    behaviortabledelegate.h \
    behaviortablewidget.h \
    horizontalheaderview.h \
    tipwidget.h

FORMS += \
    behaviortable.ui

RESOURCES += \
    res.qrc

