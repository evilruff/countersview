cmake_minimum_required(VERSION 3.16)
project(countersview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)
qt_standard_project_setup()

qt_add_executable(countersview
    src/main.cpp
    src/countersview.cpp
    src/countersmodel.cpp
    src/countersmodel.h
    src/countersstorage.cpp
    src/countersstorage.h
    src/countersview.h
    src/mutexlocker.h
)

set_target_properties(countersview PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_include_directories(countersview PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src
  	 				      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/utils
			  )

target_link_libraries(countersview PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)

