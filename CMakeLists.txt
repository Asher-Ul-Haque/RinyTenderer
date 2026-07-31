cmake_minimum_required(VERSION 3.15)
project(RinyTenderer CXX C)

# - - - 1. Enforce Clang compiler
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER   clang)
# - - - 2. Enforce C++20 standard
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# - - - 3. Output directories
# Main executable goes to project root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Tests go into bin/
set(TEST_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# - - - 4. Compiler flags
add_compile_options(
  -O0
  -g
  -Wall
  -Werror
  -Wpedantic
  -Wextra
  -Wshadow
  -Wnon-virtual-dtor
  -Wold-style-cast
  -Wunused
)

add_compile_definitions(PROFILE_ENABLED DEBUG)

# - - - 5. Vendored Ncurses
include(ExternalProject)

set(NCURSES_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/ncurses_build)
set(NCURSES_INCLUDE_DIR ${NCURSES_PREFIX}/include)
set(NCURSES_LIBRARY ${NCURSES_PREFIX}/lib/libncursesw.a)

ExternalProject_Add(ncurses_vendor
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ncurses
  PREFIX ${NCURSES_PREFIX}
  CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ncurses/configure
                      --prefix=${NCURSES_PREFIX}
                      --with-shared=no
                      --without-debug
                      --without-ada
                      --enable-widec
  BUILD_COMMAND make -j$(nproc)
  INSTALL_COMMAND make install
)

# - - - 6. Common source files
file(GLOB_RECURSE SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# - - - 7. Main application
add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    ${SRC_FILES}
)

add_dependencies(${CMAKE_PROJECT_NAME} ncurses_vendor)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${NCURSES_INCLUDE_DIR}
    ${NCURSES_INCLUDE_DIR}/ncursesw
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    ${NCURSES_LIBRARY}
)


# - - - 8. Tests
file(GLOB TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
)

foreach(TEST_FILE ${TEST_FILES})

    # Extract filename without extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME}
        ${TEST_FILE}
        ${SRC_FILES}
    )

    add_dependencies(${TEST_NAME} ncurses_vendor)

    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY}
    )

    target_include_directories(${TEST_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${NCURSES_INCLUDE_DIR}
        ${NCURSES_INCLUDE_DIR}/ncursesw
    )

    target_link_libraries(${TEST_NAME} PRIVATE
        ${NCURSES_LIBRARY}
    )

endforeach()
