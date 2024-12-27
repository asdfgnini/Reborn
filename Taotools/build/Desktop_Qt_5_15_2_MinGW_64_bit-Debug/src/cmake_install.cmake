# Install script for directory: C:/Users/zhoux/Desktop/Qt/Taotools/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/zhoux/Desktop/Qt/Taotools/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Qt/Tools/mingw810_64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/zhoux/Desktop/Qt/Taotools/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/src/libTaoTools.dll.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/zhoux/Desktop/Qt/Taotools/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/src/TaoTools.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/TaoTools.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/TaoTools.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw810_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/TaoTools.dll")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/zhoux/Desktop/Qt/Taotools/src/include/TaoGraphicsView.h"
    "C:/Users/zhoux/Desktop/Qt/Taotools/src/include/singleton.h"
    "C:/Users/zhoux/Desktop/Qt/Taotools/src/include/stdafx.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Users/zhoux/Desktop/Qt/Taotools/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/TaoTools.dll")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Users/zhoux/Desktop/Qt/Taotools/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug" TYPE SHARED_LIBRARY FILES "C:/Users/zhoux/Desktop/Qt/Taotools/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/src/TaoTools.dll")
  if(EXISTS "$ENV{DESTDIR}/C:/Users/zhoux/Desktop/Qt/Taotools/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/TaoTools.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/C:/Users/zhoux/Desktop/Qt/Taotools/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/TaoTools.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw810_64/bin/strip.exe" "$ENV{DESTDIR}/C:/Users/zhoux/Desktop/Qt/Taotools/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/TaoTools.dll")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES
    "C:/Users/zhoux/Desktop/Qt/Taotools/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/src/TaoToolsConfig.cmake"
    "C:/Users/zhoux/Desktop/Qt/Taotools/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/src/TaoToolsConfigVersion.cmake"
    )
endif()

