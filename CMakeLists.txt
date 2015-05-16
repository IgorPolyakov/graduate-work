# Store the git hash of the current head

# lukas_kanade_qt project
#
# CMAKE_BUILD_TYPE     { Debug | Release }
# INC_BUILD            { TRUE | FALSE }
# CMAKE_TOOLCHAIN_FILE filename
# CONTACT              { NONE | BISS }
# example:
# cmake -DCMAKE_BUILD_TYPE=Release -DINC_BUILD=TRUE -DCMAKE_TOOLCHAIN_FILE=~/opt/Qt/5.2.1/x86_64-w64-mingw32/bin/x86_64-w64-mingw32.toolchain
#

cmake_minimum_required(VERSION 2.8.3)
project(lukas_kanade_qt)



exec_program(
    "git"
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS "describe"
    OUTPUT_VARIABLE VERSION )

string( REGEX MATCH "-g.*$" VERSION_SHA1 ${VERSION} )
string( REGEX REPLACE "[-g]" "" VERSION_SHA1 ${VERSION_SHA1} )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/obj)

# Store the build date
if(WIN32)
  execute_process(COMMAND "cmd" " /c date /t"
    OUTPUT_VARIABLE DATE)
  string(REGEX REPLACE "[^0-9]*(..).*" "\\1" MONTH "${DATE}")
  set(MONTHS ""
    "Jan" "Feb" "Mar" "Apr" "May" "Jun"
    "Jul" "Aug" "Sep" "Oct" "Nov" "Dec")
  list(GET MONTHS "${MONTH}" MONTH)
  string(REGEX REPLACE "[^/]*/(..)/(....).*" "\\1 ${MONTH} \\2"
    PROJECT_BUILD_DATE "${DATE}")
  execute_process(COMMAND "cmd" " /c echo %TIME%"
    OUTPUT_VARIABLE TIME)
  string(REGEX REPLACE "[^0-9]*(..:..:..).*" "\\1"
    PROJECT_BUILD_TIME "${TIME}")
else()
  execute_process(COMMAND "date" "+%d %b %Y/%H:%M:%S"
    OUTPUT_VARIABLE DATE_TIME)
  string(REGEX REPLACE "([^/]*)/.*" "\\1"
    PROJECT_BUILD_DATE "${DATE_TIME}")
  string(REGEX REPLACE "[^/]*/([0-9:]*).*" "\\1"
    PROJECT_BUILD_TIME "${DATE_TIME}")
endif()


if(EXISTS "${PROJECT_SOURCE_DIR}/.git/HEAD")
  file(READ "${PROJECT_SOURCE_DIR}/.git/HEAD"
    PROJECT_SOURCE_VERSION)
  if("${PROJECT_SOURCE_VERSION}" MATCHES "^ref:")
    string(REGEX REPLACE "^ref: *([^ \n\r]*).*" "\\1"
      PROJECT_GIT_REF "${PROJECT_SOURCE_VERSION}")
    file(READ "${PROJECT_SOURCE_DIR}/.git/${PROJECT_GIT_REF}"
      PROJECT_SOURCE_VERSION)
  endif()
  string(STRIP "${PROJECT_SOURCE_VERSION}"
    PROJECT_SOURCE_VERSION)
endif()

configure_file (
  "${PROJECT_SOURCE_DIR}/src/version.h.in"
  "${PROJECT_SOURCE_DIR}/src/version.h"
  )

include("../dv/common.cmake")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

cmake_policy(SET CMP0020 NEW)

if(NOT INC_BUILD)
 set(INC_BUILD FALSE)
endif(NOT INC_BUILD)

find_package(Qt5Widgets)

include_directories(
  "${CMAKE_BINARY_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/"
  "${CMAKE_CURRENT_SOURCE_DIR}/../dv/"
  "${CMAKE_CURRENT_SOURCE_DIR}/../dv/dvio"
  "${CMAKE_CURRENT_SOURCE_DIR}/../dv/dvtypes"
)

set(INSTALL_LIB_PATH .)
set(COMPONENT_NAME BIN)
add_subdirectory("../dv/"
                 "${CMAKE_CURRENT_BINARY_DIR}/dv")

set(SOURCES
  src/lk_func.cpp
  src/main.cpp
  src/global.cpp
  src/lk_interpolation.cpp
)

set(HEADERS
  src/lk_func.h
  src/lk_struct.h
  src/global.h
  src/lk_interpolation.h
)

# Build the app!

add_definitions(-DQT5) # -fPIC)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME}
  MACOSX_BUNDLE
  WIN32
# source files that are actually built direclty
  ${SOURCES}

# items included so they show up in your IDE
  ${HEADERS}
)

# Link the app!

target_link_libraries(${PROJECT_NAME} dv)

qt5_use_modules(${PROJECT_NAME} Core Widgets)

install(TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION ${INSTALL_LIB_PATH}
        COMPONENT ${COMPONENT_NAME})

# Include CPack to introduce the appropriate targets
include(CPack)


#add_executable ( lukas_kanade_qt ${SOURCES} ${UIS} ${RSCS} ${TRS} ${MOCS} )
#target_link_libraries ( lukas_kanade_qt  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} dv )
