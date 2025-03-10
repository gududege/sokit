# ----------------------------------------------------
# sokit.pro
# ----------------------------------------------------

TEMPLATE = app
TARGET = sokit
SRC_BASE = $$PWD/../..
message(SRC_BASE=$${SRC_BASE})

QT += gui network
#CONFIG += release build_all thread
CONFIG += release
DEFINES += QT_NETWORK_LIB
INCLUDEPATH += $${SRC_BASE}/tmp $${SRC_BASE}/src/sokit
DEPENDPATH += .
UI_DIR += $${SRC_BASE}/tmp
RCC_DIR += $${SRC_BASE}/tmp

win32 {
   # DEFINES += QT_LARGEFILE_SUPPORT
    CONFIG += windows qt_static

   # QMAKE_CFLAGS_MT =-MT
   # QMAKE_CFLAGS_MT_DBG =-MTd
   # QMAKE_CFLAGS_MT_DLL =-MD
   # QMAKE_CFLAGS_MT_DLLDBG =-MDd
}

CONFIG(debug, debug|release) {
    message("Config:  Debug")
    DESTDIR = $${SRC_BASE}/bin/debug
    MOC_DIR += $${SRC_BASE}/tmp/debug
    OBJECTS_DIR += $${SRC_BASE}/tmp/debug
    INCLUDEPATH += $${SRC_BASE}/tmp/debug

    QMAKE_CFLAGS_DEBUG = $$unique(QMAKE_CFLAGS_DEBUG)
    QMAKE_CXXFLAGS_DEBUG = $$unique(QMAKE_CFLAGS_DEBUG)

    CONFIG(qt_static) {
        QMAKE_CFLAGS_DEBUG -= $$QMAKE_CFLAGS_MT_DLLDBG
        QMAKE_CFLAGS_DEBUG += $$QMAKE_CFLAGS_MT_DBG
        QMAKE_CXXFLAGS_DEBUG -= $$QMAKE_CFLAGS_MT_DLLDBG
        QMAKE_CXXFLAGS_DEBUG += $$QMAKE_CFLAGS_MT_DBG
    } else {
        QMAKE_CFLAGS_DEBUG -= $$QMAKE_CFLAGS_MT_DBG
        QMAKE_CFLAGS_DEBUG += $$QMAKE_CFLAGS_MT_DLLDBG
        QMAKE_CXXFLAGS_DEBUG -= $$QMAKE_CFLAGS_MT_DBG
        QMAKE_CXXFLAGS_DEBUG += $$QMAKE_CFLAGS_MT_DLLDBG
    }
} else {
    message("Config:  Release")
    DESTDIR = $${SRC_BASE}/bin/release
    MOC_DIR += $${SRC_BASE}/tmp/release
    OBJECTS_DIR += $${SRC_BASE}/tmp/release
    INCLUDEPATH += $${SRC_BASE}/tmp/release

    QMAKE_CFLAGS_RELEASE = $$unique(QMAKE_CFLAGS_RELEASE)
    QMAKE_CXXFLAGS_RELEASE = $$unique(QMAKE_CXXFLAGS_RELEASE)

    CONFIG(qt_static) {
        QMAKE_CFLAGS_RELEASE -= $$QMAKE_CFLAGS_MT_DLL
        QMAKE_CFLAGS_RELEASE += $$QMAKE_CFLAGS_MT
        QMAKE_CXXFLAGS_RELEASE -= $$QMAKE_CFLAGS_MT_DLL
        QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CFLAGS_MT
    } else {
        QMAKE_CFLAGS_RELEASE -= $$QMAKE_CFLAGS_MT
        QMAKE_CFLAGS_RELEASE += $$QMAKE_CFLAGS_MT_DLL
        QMAKE_CXXFLAGS_RELEASE -= $$QMAKE_CFLAGS_MT
        QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CFLAGS_MT_DLL
    }
}

HEADERS += $${SRC_BASE}/src/sokit/resource.h \
    $${SRC_BASE}/src/sokit/setting.h \
    $${SRC_BASE}/src/sokit/toolkit.h \
    $${SRC_BASE}/src/sokit/baseform.h \
    $${SRC_BASE}/src/sokit/clientform.h \
    $${SRC_BASE}/src/sokit/clientskt.h \
    $${SRC_BASE}/src/sokit/helpform.h \
    $${SRC_BASE}/src/sokit/logger.h \
    $${SRC_BASE}/src/sokit/main.h \
    $${SRC_BASE}/src/sokit/notepadform.h \
    $${SRC_BASE}/src/sokit/transferskt.h \
    $${SRC_BASE}/src/sokit/transferform.h \
    $${SRC_BASE}/src/sokit/serverskt.h \
    $${SRC_BASE}/src/sokit/serverform.h
SOURCES += $${SRC_BASE}/src/sokit/baseform.cpp \
    $${SRC_BASE}/src/sokit/clientform.cpp \
    $${SRC_BASE}/src/sokit/clientskt.cpp \
    $${SRC_BASE}/src/sokit/helpform.cpp \
    $${SRC_BASE}/src/sokit/logger.cpp \
    $${SRC_BASE}/src/sokit/main.cpp \
    $${SRC_BASE}/src/sokit/notepadform.cpp \
    $${SRC_BASE}/src/sokit/serverform.cpp \
    $${SRC_BASE}/src/sokit/serverskt.cpp \
    $${SRC_BASE}/src/sokit/setting.cpp \
    $${SRC_BASE}/src/sokit/toolkit.cpp \
    $${SRC_BASE}/src/sokit/transferform.cpp \
    $${SRC_BASE}/src/sokit/transferskt.cpp
FORMS += $${SRC_BASE}/src/sokit/clientform.ui \
    $${SRC_BASE}/src/sokit/helpform.ui \
    $${SRC_BASE}/src/sokit/serverform.ui \
    $${SRC_BASE}/src/sokit/transferform.ui
TRANSLATIONS += $${SRC_BASE}/src/sokit/sokit.ts
RESOURCES += $${SRC_BASE}/src/sokit/icons.qrc

QMAKE_PRE_LINK = lupdate $${SRC_BASE}/build/win32/sokit.pro
QMAKE_POST_LINK = lrelease $${SRC_BASE}/src/sokit/sokit.ts -qm $$DESTDIR/sokit.lan

win32 {
    RC_FILE = $${SRC_BASE}/src/sokit/sokit.rc
    LIBS += -lWs2_32 -lWinmm -lImm32
    #QMAKE_LFLAGS_DEBUG += /PDB:"$$DESTDIR/sokit.pdb"
    #QMAKE_CFLAGS_DEBUG += /Fd"$$OBJECTS_DIR/sokit.pdb"
    #QMAKE_CXXFLAGS_DEBUG += /Fd"$$OBJECTS_DIR/sokit.pdb"

   CONFIG(qt_static) {
        exists( $(QTDIR)/lib_s ) {
	   QMAKE_LIBDIR_QT = $(QTDIR)/lib_s
       }
    } else {
        exists( $(QTDIR)/lib_d ) {
	   QMAKE_LIBDIR_QT = $(QTDIR)/lib_d
       }
    }
}

