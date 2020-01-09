#-------------------------------------------------
#
# Project created by QtCreator 2020-01-08T23:38:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = batchccc
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11


# See info/icu
win32-msvc* {
    MYICUDIR = icu.msvc
    message(../$$MYICUDIR/icu4c/bin/libicuin.dll.a)
    exists( ../$$MYICUDIR/icu4c/bin/icuin.lib ) {
        MYICSVCLIB = -licutu -licuuc -licuio -licuin -licudt
    }
}
win32-g++ {
    MYICUDIR = icu.mingw730_32
    message(../$$MYICUDIR/icu4c/bin/libicuin.dll.a)
    exists(../$$MYICUDIR/icu4c/bin/libicuin.dll.a) {
        MYICSMINGWLIB = -llibicutu.dll -llibicuuc.dll -llibicudt.dll -llibicuin.dll -llibicuio.dll
    }
}


SOURCES += \
        ../../lsMisc/stdQt/stdQt.cpp \
        global.cpp \
        helper.cpp \
        main.cpp \
        mainwindow.cpp \
        taskconvert.cpp


HEADERS += \
        ../../lsMisc/stdQt/stdQt.h \
        global.h \
        helper.h \
        mainwindow.h \
        stdafx.h \
        taskconvert.h

FORMS += \
        mainwindow.ui

win32 {
    SOURCES += ../../lsMisc/stdQt/stdQt_win32.cpp \
        ../../lsMisc/GetLastErrorString.cpp \

    HEADERS += ../../lsMisc/GetLastErrorString.h
}
linux {
    SOURCES += ../../lsMisc/stdQt/stdQt_linux.cpp
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



# ICU see https://github.com/ambiesoft/autosavememo/blob/master/info/icubuild.txt.memo
win32-msvc* {
    message("MSVC")
    isEmpty(MYICSVCLIB) {
        # 1, git submodule update -i # this retrieve sources of the ICU.
        # 2, build
        # 3, copy lib/* to bin/
        # see info/ for more
        error( "ICU not ready." )
    }

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")
        LIBS += -L$$PWD/../$$MYICUDIR/icu4c/bin/ $$MYICSVCLIB

    } else {
        message("x86_64 build")
        LIBS += -L$$PWD/../$$MYICUDIR/icu4c/bin64/ $$MYICSVCLIB
    }
}
win32-g++ {
    message("win32-g++")
    isEmpty(MYICSMINGWLIB) {
        # 1, git submodule init -u # this retrieve sources of the ICU.
        # 2, build
        # 3, copy lib/* to bin/
        # see info/ for more
        error( "ICU not ready." )
    }

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")
        LIBS += -L$$PWD/../$$MYICUDIR/icu4c/bin/ $$MYICSMINGWLIB

    } else {
        message("x86_64 build")
        LIBS += -L$$PWD/../$$MYICUDIR/icu4c/bin64/ $$MYICSMINGWLIB
    }
}
linux {
    LIBS += -L/usr/lib/x86_64-linux-gnu -licui18n -licuuc -licudata
}

DISTFILES += \
    ../info/icu.txt \
    testdata/shift-jis.txt \
    testdata/shift-jis.cpp

# System libs
win32-msvc* {
    LIBS += Shlwapi.lib
}
win32-g++ {
    LIBS += -lshlwapi -lpsapi
}

message($$LIBS)

INCLUDEPATH += $$PWD/../$$MYICUDIR/icu4c/include
DEPENDPATH += $$PWD/../$$MYICUDIR/icu4c/include

message($$INCLUDEPATH)
