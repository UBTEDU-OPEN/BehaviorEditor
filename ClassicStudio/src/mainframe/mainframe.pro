# add common infomation
QT       += core gui network opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app

# add dll define
DEFINES += QT_DEPRECATED_WARNINGS \
           GLOG_NO_ABBREVIATED_SEVERITIES

equals(ROBOT_NAME, "YANSHEE1_5") {
   message(main BUILD_YANSHEE15)
   DEFINES += YANSHEE1_5
} else {
   message(main BUILD_YANSHEE11)
   DEFINES += YANSHEE1_1
}

# add dll include path
INCLUDEPATH += $${SRC}/audio \
               $${SRC}/robotcfg \
               $${SRC}/behavior \
               $${SRC}/robotview \
               $${SRC}/servoeditor \
               $${SRC}/behavioredit \
               $${SRC}/communication \
               $${SRC}/behaviortable \
               $${SRC}/updater \
               $${SRC}/commonui \
               $${SRC}/robotsearch \
               $${SRC}/datacenter \
               $${SRC}/utils

INCLUDEPATH += $${TRD}/pillow/include \
               $${TRD}/glog/include

# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET  = YansheeStudiod
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        message($${DESTDIR})
        LIBS += -L$${DESTDIR} \
                -lrobotviewd \
                -lrobotcfgd \
                -lbehaviord \
                -lservoeditord \
                -lbehavioreditd \
                -lcommunicationd \
                -lupdaterd \
                -lcommonuid \
                -lrobotsearchd \
                -ldatacenterd \
                -lutilsd
        LIBS += -L$${TRD}/pillow/lib/$${PLATE_NAME} \
                -lpillowcored
        LIBS += -L$${TRD}/glog/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglogd
    } else {
        TARGET  = YansheeStudio
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${DESTDIR} \
                -lrobotview \
                -lrobotcfg \
                -lbehavior \
                -lservoeditor \
                -lbehavioredit \
                -lcommunication \
                -lupdater \
                -lcommonui \
                -lrobotsearch\
                -ldatacenter \
                -lutils

        LIBS += -L$${TRD}/pillow/lib/$${PLATE_NAME} \
                -lpillowcore
        LIBS += -L$${TRD}/glog/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglog
    }

    # 添加管理员权限(必须加在exe对应的pro中)
#    QMAKE_LFLAGS += \"/MANIFESTUAC:level=\'requireAdministrator\' uiAccess=\'false\'\"
}

# add source/header/form/resource file
SOURCES += \
    application.cpp \
    importdialog.cpp \
    improtcanceldialog.cpp \
    improtitemwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    robotactionitem.cpp \
    robotactionlist.cpp \
    robotactionlistitem.cpp \
    robotviewchangewidget.cpp \
    settimedialog.cpp \
    templatebar.cpp \
    templatebutton.cpp \
    templatedata.cpp \
    templatedock.cpp \
    templateitem.cpp \
    templateitemdelegate.cpp \
    templatelistviewmodel.cpp \
    titlebar.cpp \
    toolbar.cpp \
    viewchangebutton.cpp \
    virtualrobotbottomwidget.cpp \
    virtualrobotwidget.cpp \
    volumeslider.cpp

HEADERS += \
    application.h \
    importdialog.h \
    improtcanceldialog.h \
    improtitemwidget.h \
    mainwindow.h \
    robotactionitem.h \
    robotactionlist.h \
    robotactionlistitem.h \
    robotviewchangewidget.h \
    settimedialog.h \
    templatebar.h \
    templatebutton.h \
    templatedata.h \
    templatedock.h \
    templateitem.h \
    templateitemdelegate.h \
    templatelistviewmodel.h \
    titlebar.h \
    toolbar.h \
    viewchangebutton.h \
    virtualrobotbottomwidget.h \
    virtualrobotwidget.h \
    volumeslider.h

FORMS += \
    importdialog.ui \
    improtcanceldialog.ui \
    improtitemwidget.ui \
    mainwindow.ui \
    robotactionlist.ui \
    robotactionlistitem.ui \
    robotviewchangewidget.ui \
    settimedialog.ui \
    templatebar.ui \
    templateitem.ui \
    titlebar.ui \
    toolbar.ui \
    virtualrobotbottomwidget.ui \
    virtualrobotwidget.ui \
    volumeslider.ui

RESOURCES += \
    res.qrc

include(../install/install.pro)

RC_ICONS = yansheestudio.ico
