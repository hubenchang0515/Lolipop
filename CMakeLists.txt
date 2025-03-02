cmake_minimum_required(VERSION 3.8)
project(Lolipop VERSION 1.0.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
  add_link_options("$<$<AND:$<STREQUAL:$<CXX_COMPILER_ID>,MSVC>,$<STREQUAL:$<CONFIG>,Release>>:/SUBSYSTEM:windows;/ENTRY:mainCRTStartup>")
  add_link_options("$<$<AND:$<STREQUAL:$<CXX_COMPILER_ID>,GNU>,$<STREQUAL:$<CONFIG>,Release>>:-mwindows>")
  set(RES "$<$<CXX_COMPILER_ID:MSVC>:src/resource/icon.rc>")
endif()

find_package(Qt6 COMPONENTS Core Widgets Multimedia MultimediaWidgets QUIET)

if (NOT Qt6_FOUND)
  find_package(Qt5 COMPONENTS Core Widgets Multimedia MultimediaWidgets REQUIRED)
endif (NOT Qt6_FOUND)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)
SET(CXX_STANDARD 11)

aux_source_directory(src SOURCE)
if (NOT Qt6_FOUND)
  message("Build on Qt5")
  add_executable(${PROJECT_NAME} ${SOURCE} src/QtTheme/QtTheme.qrc src/resource/resource.qrc ${RES})
else()
  message("Build on Qt6")
  qt_add_executable(${PROJECT_NAME} ${SOURCE} src/QtTheme/QtTheme.qrc src/resource/resource.qrc ${RES})
endif(NOT Qt6_FOUND)

target_include_directories(${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets Qt::Multimedia Qt::MultimediaWidgets)
target_link_directories(${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets Qt::Multimedia Qt::MultimediaWidgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets Qt::Multimedia Qt::MultimediaWidgets)