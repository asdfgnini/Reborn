cmake_minimum_required(VERSION 3.21)

project(TaoRGA VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# 设置编译类型为Debug，否则断点不生效
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")



#在这里配置QT路径 例如 D:/Qt/6.6.2/msvc2019_64
SET(QT_SDK_DIR C:\\Qt6.7.3\\6.7.3\\mingw_64 CACHE PATH "QT SDK DIR" FORCE)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/Install CACHE PATH "Installation path" FORCE)

list(APPEND CMAKE_PREFIX_PATH ${QT_SDK_DIR})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_subdirectory(src)


if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TaoRGA)
endif ()
