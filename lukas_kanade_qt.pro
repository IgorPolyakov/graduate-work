TEMPLATE = app
TARGET = lukas_kanade_qt
CONFIG += qt warn on debug
# Input
REVISION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --long)
DEFINES += GITHASH=\\\"$$REVISION\\\"
DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = obj
SOURCES += \
    src/lk_func.cpp \
    src/main.cpp \
    src/global.cpp
HEADERS +=  \
    src/lk_func.h\
    src/lk_struct.h \
    src/global.h
