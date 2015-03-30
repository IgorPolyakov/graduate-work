project( lukas_kanade_qt )
cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Debug )
add_definitions ( -Wall )
# Store the git hash of the current head

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

find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )
include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
	)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/obj)

set(SOURCES
	src/lk_func.cpp
	src/main.cpp
	src/global.cpp
)

set(HEADERS
	src/lk_func.h
	src/lk_struct.h
	src/global.h
)

add_executable ( lukas_kanade_qt ${SOURCES} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( lukas_kanade_qt  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} )
