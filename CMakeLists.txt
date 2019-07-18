cmake_minimum_required (VERSION 3.1)
project (VSFilterSimpleMod)

add_subdirectory(vendor/zlib)
include_directories(vendor/zlib)
add_subdirectory(vendor/libpng)
include_directories(vendor/libpng)
include_directories(${PROJECT_BINARY_DIR}/vendor/libpng)


add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_AFXDLL)
add_definitions(-D_VSMOD)
add_definitions(-D_UNICODE)
add_definitions(-D_WIN32_WINNT=0x0600)
file(GLOB CORE_FILES ${PROJECT_SOURCE_DIR}/src/subtitles/*.cpp)
file(GLOB API_FILES ${PROJECT_SOURCE_DIR}/src/vsfilter/*.cpp)

add_library(subtitles STATIC ${CORE_FILES})
add_library(VSFilterSimpleMod SHARED ${API_FILES})
target_link_libraries(VSFilterSimpleMod zlibstatic png_static subtitles)

set (VERSION "r1")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_dependencies(png_static zlibstatic)
add_dependencies(subtitles png_static)

add_custom_command(
  TARGET VSFilterSimpleMod POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:VSFilterSimpleMod> "../Release_${VERSION}/${_DIR}/$<TARGET_FILE_NAME:VSFilterSimpleMod>"
)

SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
