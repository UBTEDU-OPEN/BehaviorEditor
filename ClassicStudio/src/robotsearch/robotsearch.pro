QT += gui network widgets

TEMPLATE = lib
DEFINES += ROBOTSEARCH_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH +=$${SRC}/communication\
            $${SRC}/robotcfg
-robotcfg
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32{
    CONFIG(debug, debug|release){
        TARGET  = robotsearchd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS    += -L$${DESTDIR} \
                   -lcommunicationd\
                   -lrobotcfgd


    }else{
        TARGET  = robotsearch
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS    += -L$${DESTDIR} \
                   -lcommunication\
                   -lrobotcfg

    }
}

SOURCES += \
    connectedwidget.cpp \
    connectionsuccesswidget.cpp \
    inputsnwidget.cpp \
    robotlistwidget.cpp \
    robotsearch.cpp \
    robotsearchdialog.cpp \
    searchingwidget.cpp

HEADERS += \
    connectedwidget.h \
    connectionsuccesswidget.h \
    inputsnwidget.h \
    robotlistwidget.h \
    robotsearch_global.h \
    robotsearch.h \
    robotsearchdialog.h \
    searchingwidget.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    connectedwidget.ui \
    connectionsuccesswidget.ui \
    inputsnwidget.ui \
    robotlistwidget.ui \
    robotsearchdialog.ui \
    searchingwidget.ui

RESOURCES += \
    res.qrc

DISTFILES +=
