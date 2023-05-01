# -----------------------------------------------------------------------------
# Usage
# -----------------------------------------------------------------------------
#
# Run generator:
# $ cmake -S. -Bbuild
#
# Build all:
# $ cmake --build build
#
# Build specific target:
# $ cmake --build build --target clean
# $ cmake --build build --target toolbrush -- -j
# $ cmake --build build --target src -- -j
#
# Build with specific option:
# $ cmake --build build -- VERBOSE=1
# $ cmake --build build -- -j 12
# -----------------------------------------------------------------------------

cmake_minimum_required( VERSION 3.2 )

include( CMakePrintHelpers )

project( phidi LANGUAGES CXX )

if( POLICY CMP0076 )
    cmake_policy( SET CMP0076 NEW )
endif()

set( CMAKE_BUILD_TYPE Debug )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_EXTENSIONS OFF )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# -----------------------------------------------------------------------------
# Generate compile_commands.json for LSP
# -----------------------------------------------------------------------------
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# -----------------------------------------------------------------------------
# Find SFML
# -----------------------------------------------------------------------------
find_package( SFML COMPONENTS system window graphics REQUIRED )
set( SFML_LIBRARIES sfml-system sfml-window sfml-graphics )
cmake_print_variables( SFML_LIBRARIES )

# -----------------------------------------------------------------------------
# Remove stupid 'lib' prefix produced by add_library()
# -----------------------------------------------------------------------------
set( CMAKE_STATIC_LIBRARY_PREFIX "" )
set( CMAKE_SHARED_LIBRARY_PREFIX "" )

# -----------------------------------------------------------------------------
# Setup flags
# -----------------------------------------------------------------------------
include( cmake/txx.cmake )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${TXX_FLAGS}" )
set( CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${TXX_FLAGS}")

# -----------------------------------------------------------------------------
# Change compiled *.so and executables location
# -----------------------------------------------------------------------------
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

# -----------------------------------------------------------------------------
# It is not recommended using GLOB to collect a list of source files
# from your source tree.
#
# https://cmake.org/cmake/help/latest/command/file.html#glob
# -----------------------------------------------------------------------------

add_subdirectory( plugins/init )
add_subdirectory( plugins/textures )
add_subdirectory( plugins/fonts )
add_subdirectory( plugins/tools )
add_subdirectory( plugins/toolbrush )
add_subdirectory( plugins/tool_palette )
# add_subdirectory( plugins/property_palette )

add_executable( phi main.cc )
target_include_directories( phi PUBLIC ${PROJECT_SOURCE_DIR} )

add_subdirectory( core )
add_subdirectory( widgets )

target_link_libraries( phi
    PUBLIC
        widgets
    PRIVATE
        core
        sfml-graphics
)
