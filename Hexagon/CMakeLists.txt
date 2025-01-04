cmake_minimum_required(VERSION 3.16)
project(Hexagon1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)


include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}
)


set(QT_DIR "/home/holger/Qt/6.8.0/gcc_64")
set(CMAKE_PREFIX_PATH "${QT_DIR}")
set(QT_PATH "/home/holger/Qt/6.8.0/gcc_64")
find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia REQUIRED)
set(CMAKE_AUTOUIC ON)

#UI Files
set(UI_FILES
        mainwindow.ui
        headquarter.ui
        combatdialog.ui
        startscreen.ui
        mapSizeDialog.ui
        )

# Header-files including MOC
set(HEADER_FILES
        mainwindow.h
        headquarterdialog.h
        combatdialog.h
        hexitemsignalhelper.h
        zoomablegraphicsview.h
        ClickableGraphicsView.h
        startscreen.h
        mapsizedialog.h
        customdialog.h
        aimanager.h
        stylemanager.h
        )

# Source-files
set(SOURCES
        main.cpp
        mainwindow.cpp
        headquarterdialog.cpp
        combatdialog.cpp
        hexitemsignalhelper.cpp
        zoomablegraphicsview.cpp
        ClickableGraphicsView.cpp
        fieldtype.cpp
        hex.cpp
        hexitem.cpp
        hexmap.cpp
        unit.cpp
        unittype.cpp
        startscreen.cpp
        mapsizedialog.cpp
        customdialog.cpp
        aimanager.cpp
        stylemanager.cpp
        )

# Add resources
qt_add_resources(RESOURCES Images.qrc)

# Automatic Generation of MOC-files and UI-files
qt_wrap_ui(UI_GENERATED ${UI_FILES})
qt_wrap_cpp(MOC_GENERATED ${HEADER_FILES})


add_executable(Hexagon1
        ${SOURCES}
        ${UI_GENERATED}
        ${MOC_GENERATED}
        ${RESOURCES}
        )

target_link_libraries(Hexagon1 Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia)