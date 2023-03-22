# 拷贝第三方动态库至程序运行目录下
QMAKE_POST_LINK += \
    xcopy /y /s /e /i  \"$${TRD}\msvcp\\$${PLATE_NAME}\\$${DIR_NAME}\*.dll\"             \"$${DESTDIR}\"                 && \
    xcopy /y /s /e /i  \"$${TRD}\fbxsdk\lib\\$${PLATE_NAME}\\$${DIR_NAME}\*.dll\"        \"$${DESTDIR}\"                 && \
    xcopy /y /s /e /i  \"$${TRD}\7z\\$${PLATE_NAME}\*.*\"                                \"$${DESTDIR}\"                 && \
    xcopy /y /s /e /i  \"$${TRD}\ffmpeg\\$${PLATE_NAME}\lib\*.dll\"                      \"$${DESTDIR}\"                 && \
    xcopy /y /s /e /i  \"$${TRD}\glog\lib\\$${PLATE_NAME}\\$${DIR_NAME}\*.dll\"          \"$${DESTDIR}\"

# 拷贝资源文件
QMAKE_POST_LINK += && \
    xcopy /y /s /e /i  \"$${SRC}\resource\robot\*.*\"                        \"$${DESTDIR}\resource\robot\"              && \
    xcopy /y /s /e /i  \"$${SRC}\resource\libraries\*.*\"                    \"$${DESTDIR}\resource\libraries\"          && \
    xcopy /y /s /e /i  \"$${SRC}\configs\*.*\"                               \"$${DESTDIR}\configs\"                     && \
    copy /y            \"$${SRC}\resource\qt.conf\"                          \"$${DESTDIR}\qt.conf\"                     && \
    copy /y            \"$${SRC}\resource\help.pdf\"                         \"$${DESTDIR}\resource\help.pdf\"           && \
    copy /y            \"$${SRC}\yansheestudio.qm\"                          \"$${DESTDIR}\yansheestudio.qm\"            && \
    copy /y            \"$${SRC}\mainframe\yansheestudio.ico\"               \"$${DESTDIR}\"


# 拷贝Qt运行库至程序运行目录下
CONFIG(debug, debug|release){
QMAKE_POST_LINK += && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Guid.dll\"                                  \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Networkd.dll\"                              \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Widgetsd.dll\"                              \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Cored.dll\"                                 \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5OpenGLd.dll\"                               \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Xmld.dll\"                                  \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Svgd.dll\"                                  \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Multimediad.dll\"                           \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Quickd.dll\"                                \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5PrintSupportd.dll\"                         \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Qmld.dll\"                                  \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5QuickWidgetsd.dll\"                         \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Positioningd.dll\"                          \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5QmlModelsd.dll\"                            \"$${DESTDIR}\*.dll\"              && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\platforms\qwindowsd.dll\"                  \"$${DESTDIR}\platforms\*.dll\"    && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\imageformats\qsvgd.dll\"                   \"$${DESTDIR}\imageformats\*.dll\" && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\imageformats\qgifd.dll\"                   \"$${DESTDIR}\imageformats\*.dll\" && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\mediaservice\dsengined.dll\"               \"$${DESTDIR}\mediaservice\*.dll\" && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\mediaservice\qtmedia_audioengined.dll\"    \"$${DESTDIR}\mediaservice\*.dll\" && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\mediaservice\wmfengined.dll\"              \"$${DESTDIR}\mediaservice\*.dll\"
}else{
QMAKE_POST_LINK += && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Gui.dll\"                                   \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Network.dll\"                               \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Widgets.dll\"                               \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Core.dll\"                                  \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5OpenGL.dll\"                                \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Xml.dll\"                                   \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Svg.dll\"                                   \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Multimedia.dll\"                            \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Quick.dll\"                                 \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5PrintSupport.dll\"                          \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Qml.dll\"                                   \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5QuickWidgets.dll\"                          \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5Positioning.dll\"                           \"$${DESTDIR}\*.dll\"              && \
    copy  /y            \"$$(QTDIR)\bin\Qt5QmlModels.dll\"                             \"$${DESTDIR}\*.dll\"              && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\platforms\qwindows.dll\"                   \"$${DESTDIR}\platforms\*.dll\"    && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\imageformats\qsvg.dll\"                    \"$${DESTDIR}\imageformats\*.dll\" && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\imageformats\qgif.dll\"                    \"$${DESTDIR}\imageformats\*.dll\" && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\mediaservice\dsengine.dll\"               \"$${DESTDIR}\mediaservice\*.dll\"  && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\mediaservice\qtmedia_audioengine.dll\"    \"$${DESTDIR}\mediaservice\*.dll\"  && \
    xcopy /y /s /e /i   \"$$(QTDIR)\plugins\mediaservice\wmfengine.dll\"              \"$${DESTDIR}\mediaservice\*.dll\"
}
