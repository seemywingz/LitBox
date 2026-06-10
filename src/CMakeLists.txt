# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

# Base requirements for all builds
set(COMPONENT_REQUIRES esp_littlefs )

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

idf_component_register(SRCS ${app_sources})

idf_component_register(
    SRCS ${app_sources}
    REQUIRES ${COMPONENT_REQUIRES}
)