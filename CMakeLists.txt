cmake_minimum_required(VERSION 3.28)

project(derklib VERSION 0.1.0 LANGUAGES CXX)

set(DERKLIB_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/derklib/includes)
set(DERKLIB_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

if (DEBUG_MODE)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -g -Og)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O3)
endif ()

enable_testing()
# add_subdirectory(derklib)
# add_subdirectory(samples)
add_subdirectory(tests)
