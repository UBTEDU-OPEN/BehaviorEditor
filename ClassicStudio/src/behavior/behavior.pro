# add common infomation
QT      -= core gui
TEMPLATE = lib

# add dll define
DEFINES +=  BEHAVIOR_EXPORT

# add dll include path

# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET   = behaviord
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    } else {
        TARGET   = behavior
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
}

# add source/header/form/resource file
SOURCES +=  tinyxml2.cpp \
            behavior.cpp

HEADERS +=  tinyxml2.h \
            behavior.h

#屏蔽告警
QMAKE_CXXFLAGS_WARN_ON += -wd4251
