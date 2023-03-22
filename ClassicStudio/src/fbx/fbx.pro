# add common infomation
QT      -= core gui
TEMPLATE = lib

# add dll define
DEFINES +=  FBX_EXPORT \
            FBXSDK_SHARED

# add dll include path
INCLUDEPATH += $${TRD}/fbxsdk/include

# windows plate
win32{
    CONFIG(debug, debug|release){
        TARGET   = fbxd
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${TRD}/fbxsdk/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglew32 \
                -lglut32 \
                -lglu32 \
                -lopengl32 \
                -llibfbxsdk
    } else {
        TARGET   = fbx
        DESTDIR = $${BIN}/$${PLATE_NAME}/$${DIR_NAME}
        LIBS += -L$${TRD}/fbxsdk/lib/$${PLATE_NAME}/$${DIR_NAME} \
                -lglew32 \
                -lglut32 \
                -lglu32 \
                -lopengl32 \
                -llibfbxsdk
    }
}

# add source/header/form/resource file
SOURCES +=  DrawScene.cxx \
            DrawText.cxx \
            GetPosition.cxx \
            GlFunctions.cxx \
            SceneCache.cxx \
            SceneContext.cxx \
            SetCamera.cxx \
            targa.cxx \
            Common/Common.cxx \
            fbx.cpp

HEADERS +=  DrawScene.h \
            DrawText.h \
            GetPosition.h \
            GlFunctions.h \
            SceneCache.h \
            SceneContext.h \
            SetCamera.h \
            targa.h \
            Common/Common.h \
            global.h \
            fbx.h
