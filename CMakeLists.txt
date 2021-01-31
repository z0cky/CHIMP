cmake_minimum_required(VERSION 3.1)

project(CHIMP VERSION 1.0.0)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Generates UI files (.ui file)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Instruct CMake to run uic automatically when needed.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${PROJECT_SOURCE_DIR}/form")

# Generate resource files (.qrc file)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTORCC_SEARCH_PATHS "${PROJECT_SOURCE_DIR}/resource")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g")

# including Qt5Widgets makes the next two lines redundant
# find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
# find_package(Qt5Core REQUIRED)

find_package(Qt5Widgets REQUIRED)

# include_directories(src)
# include_directories(hdr)
# include_directories(form)
# include_directories(resource)

set(
    SOURCES
    src/main.cpp
    src/content_view.cpp
    src/imageHistory.cpp
    src/mainwindow.cpp
    src/tools.cpp
    src/histogram.cpp
    src/parameters.cpp
    src/painter.cpp
)

set(
    HEADERS
    hdr/mainwindow.h
    hdr/constants.h
    hdr/content_view.h
    hdr/imageHistory.h
    hdr/histogram.h
    hdr/parameters.hpp
    hdr/parameters.hpp
    hdr/tools.h
    hdr/painter.h
)

set(
    UI
    form/mainwindow.ui
    form/penwidthdialog.ui
    form/resizewindow.ui
)

set(
    RESOURCE
    resource/icons.qrc
    resource/StyleResources.qrc 
)

add_executable(${PROJECT_NAME} ${RESOURCE} ${UI} ${HEADERS} ${SOURCES})
target_link_libraries(CHIMP PUBLIC Qt5::Widgets)
