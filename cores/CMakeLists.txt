cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.12.0)

project(core)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_DEBUG_POSTFIX -dbg)
set(CMAKE_RELEASE_POSTFIX -rel)

add_subdirectory(nmsdk2)
add_subdirectory(target)