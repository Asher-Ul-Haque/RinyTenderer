cmake_minimum_required(VERSION 3.15)
project(RinyTenderer CXX C)

# - - - 1. Enforce Clang compiler
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER   clang)

# - - - 2. Enforce C++20 standard
set(CMAKE_CXX_STANDARD            20)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_EXTENSIONS          OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# - - - 3. Output the final binary directly in the project root folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# - - - 4. Strict Debug and Warning Flags (-O0, -g, -Wall, -Werror, -Wpedantic)
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
  -Wunused)

# - - - 5. Handle the Vendored Ncurses Submodule
include(ExternalProject)

set(NCURSES_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/ncurses_build)
set(NCURSES_INCLUDE_DIR ${NCURSES_PREFIX}/include)
set(NCURSES_LIBRARY ${NCURSES_PREFIX}/lib/libncursesw.a) # Note the 'w' for widechar

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

# - - - 6. Define Target Application
file(GLOB_RECURSE SRC_FILES src/*.cpp main.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
add_dependencies(${CMAKE_PROJECT_NAME} ncurses_vendor)

# - - - 7. Include Directories & Linker Actions
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${NCURSES_INCLUDE_DIR}
  ${NCURSES_INCLUDE_DIR}/ncursesw
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${NCURSES_LIBRARY})
