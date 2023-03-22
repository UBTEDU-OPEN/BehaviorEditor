win32 {
    include(install_windows.pro)
}
unix {
    macx {
        include(install_macx.pro)
    } else {
        include(install_linux.pro)
    }
}
