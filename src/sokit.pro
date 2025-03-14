# ----------------------------------------------------
# sokit.pro
# ----------------------------------------------------

TEMPLATE = app
TARGET = sokit
VERSION = 1.3.2

SRC_BASE = $$PWD/..

QT += gui network
#CONFIG += release build_all thread
CONFIG += release
DEFINES += QT_NETWORK_LIB
INCLUDEPATH += $${SRC_BASE}/tmp $${SRC_BASE}/src
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

HEADERS += $${SRC_BASE}/src/resource.h \
    $${SRC_BASE}/src/setting.h \
    $${SRC_BASE}/src/toolkit.h \
    $${SRC_BASE}/src/baseform.h \
    $${SRC_BASE}/src/clientform.h \
    $${SRC_BASE}/src/clientskt.h \
    $${SRC_BASE}/src/helpform.h \
    $${SRC_BASE}/src/logger.h \
    $${SRC_BASE}/src/main.h \
    $${SRC_BASE}/src/notepadform.h \
    $${SRC_BASE}/src/transferskt.h \
    $${SRC_BASE}/src/transferform.h \
    $${SRC_BASE}/src/serverskt.h \
    $${SRC_BASE}/src/serverform.h
SOURCES += $${SRC_BASE}/src/baseform.cpp \
    $${SRC_BASE}/src/clientform.cpp \
    $${SRC_BASE}/src/clientskt.cpp \
    $${SRC_BASE}/src/helpform.cpp \
    $${SRC_BASE}/src/logger.cpp \
    $${SRC_BASE}/src/main.cpp \
    $${SRC_BASE}/src/notepadform.cpp \
    $${SRC_BASE}/src/serverform.cpp \
    $${SRC_BASE}/src/serverskt.cpp \
    $${SRC_BASE}/src/setting.cpp \
    $${SRC_BASE}/src/toolkit.cpp \
    $${SRC_BASE}/src/transferform.cpp \
    $${SRC_BASE}/src/transferskt.cpp
FORMS += $${SRC_BASE}/src/ui/clientform.ui \
    $${SRC_BASE}/src/ui/helpform.ui \
    $${SRC_BASE}/src/ui/serverform.ui \
    $${SRC_BASE}/src/ui/transferform.ui
TRANSLATIONS += $${SRC_BASE}/src/i18n/sokit-cn.ts
RESOURCES += $${SRC_BASE}/src/res/icons.qrc

QMAKE_PRE_LINK = lupdate $${SRC_BASE}/src/sokit.pro
QMAKE_POST_LINK = lrelease $${SRC_BASE}/src/i18n/sokit-cn.ts -qm $$DESTDIR/sokit-cn.lan

win32 {
    RC_FILE = $${SRC_BASE}/src/res/sokit.rc
    LIBS += -lWs2_32 -lWinmm -lImm32
    #QMAKE_LFLAGS_DEBUG += /PDB:"$$DESTDIR/sokit.pdb"
    #QMAKE_CFLAGS_DEBUG += /Fd"$$OBJECTS_DIR/sokit.pdb"
    #QMAKE_CXXFLAGS_DEBUG += /Fd"$$OBJECTS_DIR/sokit.pdb"

   CONFIG(qt_static) {
        exists( $(QTDIR)/lib/static ) {
	   QMAKE_LIBDIR_QT = $(QTDIR)/lib/static 
       }
    } else {
        exists( $(QTDIR)/lib/shared ) {
	   QMAKE_LIBDIR_QT = $(QTDIR)/lib/shared 
       }
    }
}

