cmake_minimum_required(VERSION 3.21)

project(17RGA)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};D:\\Qt\\6.7.3\\llvm-mingw_64\\lib\\cmake")

# 查找 Qt6 模块，确保 Core 和 Widgets 被找到
find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport Network Concurrent)
# find_package(Qt5 REQUIRED COMPONENTS Core Widgets PrintSupport Network Concurrent)
# 检查 Qt6 是否成功找到，并输出相应的提示信息

if(Qt6_FOUND)
    message(STATUS "Qt6 Core and Widgets found successfully.")
else()
    message(FATAL_ERROR "Qt6 Core and/or Widgets not found!")
endif()
# if(Qt5_FOUND)
#     message(STATUS "Qt5 Core and Widgets found successfully.")
# else()
#     message(FATAL_ERROR "Qt5 Core and/or Widgets not found!")
# endif()

# 配置 Qt 项目环境，使用 Qt 标准项目设置
qt_standard_project_setup()

# 输出 qt_standard_project_setup 执行后的状态信息
message(STATUS "Qt standard project setup complete.")

# 主程序
add_subdirectory(src)


