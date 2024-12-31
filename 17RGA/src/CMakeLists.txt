
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mconsole")  # 设定以控制台方式编译还是以窗口模式编译

FILE(GLOB_RECURSE ORIGIN *.h *.cpp)
# FILE(GLOB INCLUDE include/*.h)

# message("ORIGIN: ${ORIGIN}")
# message("INCLUDE: ${INCLUDE}")

# 添加可执行文件
add_executable(${PROJECT_NAME} ${ORIGIN})


# 链接第三方库
target_link_libraries(${PROJECT_NAME} PRIVATE  
    Qt6::Core
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::Network
    Qt6::Concurrent
)
