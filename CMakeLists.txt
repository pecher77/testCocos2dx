#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(CMAKE_OSX_ARCHITECTURES x86_64)
set(APP_NAME ScorewarriorTest)

project(${APP_NAME})

if (XCODE)
    if (NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        SET(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif ()
endif ()

if (NOT DEFINED BUILD_ENGINE_DONE) # to test ScorewarriorTest into root project
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif ()

# record sources, headers, resources...
set(GAME_RES_FOLDER
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
)
if (APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif ()

set(PLATFORM_SPECIFIC_SRC)
set(PLATFORM_SPECIFIC_HEADERS)

if (WIN32)
    list(APPEND PLATFORM_SPECIFIC_HEADERS
            proj.win32/main.h
            proj.win32/resource.h
            proj.win32/stdafx.h
    )
    list(APPEND PLATFORM_SPECIFIC_SRC
            proj.win32/main.cpp
            proj.win32/game.rc
            ${common_res_files}
    )
elseif (MACOSX OR APPLE OR LINUX)
    set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
    )
    list(APPEND PLATFORM_SPECIFIC_SRC
            proj.ios_mac/mac/main.cpp
            proj.ios_mac/mac/Prefix.pch
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            ${common_res_files}
    )
elseif (ANDROID)
    set(PLATFORM_SPECIFIC_SRC
            proj.android-studio/app/jni/hellocpp/main.cpp
    )
endif ()

# mark app complie info and libs info

file(GLOB_RECURSE GAME_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Classes/*.cpp")
file(GLOB_RECURSE GAME_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Classes/*.h")

set(GAME_SRC
        ${GAME_SRC}
        ${PLATFORM_SPECIFIC_SRC}
)

set(GAME_HEADERS
        ${GAME_HEADERS}
        ${PLATFORM_SPECIFIC_HEADERS})

if (ANDROID)
    add_library(${APP_NAME} SHARED ${GAME_SRC} ${GAME_HEADERS})

    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
else ()
    add_executable(${APP_NAME} ${GAME_SRC} ${GAME_HEADERS})
endif ()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if (APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    set_xcode_property(${APP_NAME} INSTALL_PATH "\$(LOCAL_APPS_DIR)")

    if (MACOSX)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")
        set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")

    elseif (IOS)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif ()

    # For code-signing, set the DEVELOPMENT_TEAM:
    #set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "GRLXXXX2K9")
elseif (WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif ()

if (LINUX OR WINDOWS)
    cocos_get_resource_path(APP_RES_DIR ${APP_NAME})
    cocos_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif ()
