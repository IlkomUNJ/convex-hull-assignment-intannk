cmake_minimum_required(VERSION 3.16)

project(tugas_komgraf VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(tugas_komgraf
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        drawingwidget.cpp
        drawingwidget.h
    )
else()
    add_executable(tugas_komgraf
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(tugas_komgraf PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(tugas_komgraf)
endif()
