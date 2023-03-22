# add common infomation
QT       = core xml
QT      -= gui
TEMPLATE = lib

# add dll define
DEFINES += QT_DEPRECATED_WARNINGS \
           ROBOTCFG_LIBRARY


# add dll include path


# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET  = robotcfgd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    } else {
        TARGET   = robotcfg
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
}

# add source/header/form/resource file
SOURCES += robotcfg.cpp \
    classstudiosettings.cpp \
    predefinedposture.cpp

HEADERS += robotcfg.h\
           classstudiosettings.h \
           predefinedposture.h \
           robotcfg_global.h
