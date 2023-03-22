# add common infomation
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = lib

# add dll define
DEFINES += QT_DEPRECATED_WARNINGS \
           SERVOEDITOR_LIBRARY

# add dll include path
INCLUDEPATH += $${SRC}/servoeditor \
               $${SRC}/robotcfg \
               $${SRC}/commonui \
               $${SRC}/datacenter \
               $${SRC}/behavior \
               $${SRC}/utils \
               $${TRD}/glog/include

# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET  = servoeditord
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lrobotcfgd \
                -lcommonuid \
                -ldatacenterd \
                -lutilsd
        LIBS += -L$${TRD}/glog/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglogd
    } else {
        TARGET  = servoeditor
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
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
    dangerousdialog.cpp \
    doubleclickedbutton.cpp \
    jointadjuestmentwidget.cpp \
    jointselectorwidget.cpp \
    jointwidget.cpp \
    servoeditor.cpp \
    servoform.cpp

HEADERS += \
    dangerousdialog.h \
    doubleclickedbutton.h \
    jointadjuestmentwidget.h \
    jointselectorwidget.h \
    jointwidget.h \
    servoeditor_global.h \
    servoeditor.h \
    servoform.h

FORMS += \
    dangerousdialog.ui \
    jointadjuestmentwidget.ui \
    jointselectorwidget.ui \
    jointwidget.ui \
    servoeditor.ui \
    servoform.ui

RESOURCES += \
    res.qrc
