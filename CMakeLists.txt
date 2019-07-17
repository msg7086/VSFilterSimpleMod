cmake_minimum_required (VERSION 3.1)
project (VSFilterSimpleMod)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
file(GLOB ZLIB_FILES ${PROJECT_SOURCE_DIR}/src/zlib/*.c)
file(GLOB PNG_FILES ${PROJECT_SOURCE_DIR}/src/libpng/*.c)

add_definitions(-D_AFXDLL)
add_definitions(-D_VSMOD)
add_definitions(-D_UNICODE)
add_definitions(-D_WIN32_WINNT=0x0600)
file(GLOB CORE_FILES ${PROJECT_SOURCE_DIR}/src/subtitles/*.cpp)
file(GLOB API_FILES ${PROJECT_SOURCE_DIR}/src/vsfilter/*.cpp)

add_library(zlib STATIC ${ZLIB_FILES})
add_library(libpng STATIC ${PNG_FILES})
add_library(subtitles STATIC ${CORE_FILES})
add_library(VSFilterSimpleMod SHARED ${API_FILES})
target_link_libraries(VSFilterSimpleMod zlib libpng subtitles)

set (VERSION "r1")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src/zlib)
include_directories(${PROJECT_SOURCE_DIR}/src/libpng)
add_custom_command(
  TARGET VSFilterSimpleMod POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:VSFilterSimpleMod> "../Release_${VERSION}/${_DIR}/$<TARGET_FILE_NAME:VSFilterSimpleMod>"
)
