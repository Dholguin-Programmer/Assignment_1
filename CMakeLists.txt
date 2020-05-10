cmake_minimum_required(VERSION 3.11)
project(OpenGL_Project_1 VERSION 1.0 LANGUAGES C CXX)
string(TOUPPER ${CMAKE_PROJECT_NAME} UPPER_PROJECT_NAME)
string(TOLOWER ${CMAKE_PROJECT_NAME} LOWER_PROJECT_NAME)
set(3RD_PARTY_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/3rdPartyLibs")

################################################################################
# Fetch toolchain from environment variable (TODO: Provide different toolchains)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(AssureOutOfSourceBuild)
include(install-helpers)
set(CMAKE_BUILD_TYPE Debug)

################################################################################
# 3rd Party Build & Pre-build library linking

# Embed the GLFW library into our build
set(GLFW_DIR "${3RD_PARTY_LIBS}/glfw")
set(GLFW_INCLUDE_DIRECTORIES "${GLFW_DIR}/include")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")

# Add pre-existing in-source built GLAD library (Used for generating a OpenGL spec)
set(GLAD_DIR "${3RD_PARTY_LIBS}/glad")
set(GLAD_INCLUDE_DIRECTORIES "${GLAD_DIR}/include")
set(GLAD_TARGET "GLAD")
add_library(${GLAD_TARGET} "${GLAD_DIR}/src/glad.c")
target_include_directories("GLAD" PRIVATE "${GLAD_INCLUDE_DIRECTORIES}")

# Embed the GLM library into our build
set(GLM_DIR "${3RD_PARTY_LIBS}/glm")
add_subdirectory(${GLM_DIR})

# Embed the JSON Parser library into our build
set(JSON_DIR "${3RD_PARTY_LIBS}/json")
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(${JSON_DIR})

################################################################################

# Build Assignment
add_subdirectory(Devices/Window)
add_subdirectory(App)