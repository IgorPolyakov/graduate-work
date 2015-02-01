TEMPLATE = app
TARGET =
DEPENDPATH += . src
INCLUDEPATH += .
# Input
REVISION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe)
DEFINES += GITHASH=\\\"$$REVISION\\\"
SOURCES += src/main.cpp
