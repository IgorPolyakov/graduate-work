TEMPLATE = app
TARGET =
DEPENDPATH += . src
INCLUDEPATH += .
# Input
REVISION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --long)
DEFINES += GITHASH=\\\"$$REVISION\\\"
SOURCES += \
    src/lk_func.cpp \
    src/main.cpp \
    src/global.cpp
HEADERS +=  \
    src/lk_func.h\
    src/lk_struct.h \
    src/global.h
