#
# Copyright (c) 2025-present Henri Michelon
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.29)

#######################################################
project(vireo_tutorial_1)
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.env.cmake")
    message(FATAL_ERROR "Please create a .env.cmake file with the VIREO_PROJECT_DIR variable")
endif ()
include(.env.cmake)
if (NOT DEFINED VIREO_PROJECT_DIR)
    message(FATAL_ERROR "Please set VIREO_PROJECT_DIR in the .env.cmake file")
endif()

#######################################################
if(WIN32)
    set(DIRECTX_BACKEND ON)
endif ()

#######################################################
add_subdirectory(${VIREO_PROJECT_DIR} external_lib_build)
set(VIREO_INCLUDE_DIR ${VIREO_PROJECT_DIR}/include)
set(VIREO_TARGET "vireo_rhi")

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SHADERS_SRC_DIR ${SRC_DIR}/shaders)
set(SHADERS_INCLUDE_DIR ${SRC_DIR}/shaders/include)
set(SHADERS_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

#######################################################
include(FetchContent)
include(cmake/shaders.cmake)
include(cmake/libraries.cmake)

#######################################################
# Slang shaders
file(MAKE_DIRECTORY ${SHADERS_BUILD_DIR})
file(GLOB_RECURSE SHADERS_SOURCE_FILES
        "${SHADERS_SRC_DIR}/*.slang"
)
add_shaders(shaders ${SHADERS_BUILD_DIR} ${SHADERS_INCLUDE_DIR} ${SHADERS_SOURCE_FILES} )

#######################################################
# Platform specific files
if (WIN32)
    set(WIN32_SOURCES
            ${SRC_DIR}/platform/windows/Win32Application.cpp
    )
    set(WIN32_MODULES
            ${SRC_DIR}/platform/windows/Win32Application.ixx
    )
elseif (UNIX AND NOT APPLE)
    set(LINUX_SOURCES
            ${SRC_DIR}/platform/sdl/SDLApplication.cpp
    )
    set(LINUX_MODULES
            ${SRC_DIR}/platform/sdl/SDLApplication.ixx
    )
endif ()

#######################################################
# Common files
set(COMMON_SOURCES
)
set(COMMON_MODULES
        ${SRC_DIR}/Application.ixx
)

#######################################################
function(build_target TARGET_NAME SRCS MODULES)
    message("Building target ${TARGET_NAME}")
    add_executable(${TARGET_NAME}
            ${LINUX_SOURCES}
            ${WIN32_SOURCES}
            ${COMMON_SOURCES}
            ${SRCS})
    target_sources(${TARGET_NAME}
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
            ${LINUX_MODULES}
            ${WIN32_MODULES}
            ${COMMON_MODULES}
            ${MODULES}
    )
    vireo_compile_options(${TARGET_NAME})
    target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDE_DIR} ${VIREO_INCLUDE_DIR})
    target_link_libraries(${TARGET_NAME} ${VIREO_TARGET} glm::glm glm-modules)
    add_dependencies(${TARGET_NAME} ${VIREO_TARGET} shaders)
endfunction()

#######################################################
set(MY_TARGET_SRC
        ${SRC_DIR}/MyApp.cpp
        ${SRC_DIR}/MyAppMain.cpp
)
set(MY_TARGET_MODULES
        ${SRC_DIR}/MyApp.ixx
)
build_target(myapp "${MY_TARGET_SRC}" ${MY_TARGET_MODULES})
