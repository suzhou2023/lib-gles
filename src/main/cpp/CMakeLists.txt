cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_NAME gl-common)

project(${PROJECT_NAME} VERSION 1.0)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/${ANDROID_ABI}")


# 头文件
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/common")


# opengl相关api编译成so
add_library(
        ${PROJECT_NAME}

        SHARED

        jni/jni.cpp
        common/GLContext.cpp
        common/asset_util.cpp
        common/egl_util.cpp
        common/fbo_util.cpp
        common/gl_util.cpp
        common/shader_util.cpp
)

# 链接库
target_link_libraries(
        ${PROJECT_NAME}

        # for openGLES
        GLESv3
        # for EGL
        EGL
        android
        jnigraphics
        # android log
        log
)


add_subdirectory(3d)





















