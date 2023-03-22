QT += widgets

TEMPLATE = lib
DEFINES += COMMONUI_LIBRARY

CONFIG += c++11

# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET   = commonuid
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    } else {
        TARGET   = commonui
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
    }
}

SOURCES += \
    combobutton.cpp \
    commondialog.cpp \
    nameinputwidget.cpp \
    savefiledialog.cpp \
    spinbox.cpp \
    toastdialog.cpp \
    widgettipdialog.cpp

HEADERS += \
    combobutton.h \
    commondialog.h \
    commonui_global.h \
    nameinputwidget.h \
    savefiledialog.h \
    spinbox.h \
    toastdialog.h \
    widgettipdialog.h

RESOURCES += \
    res.qrc

FORMS += \
    commondialog.ui \
    nameinputwidget.ui \
    savefiledialog.ui \
    toastdialog.ui
