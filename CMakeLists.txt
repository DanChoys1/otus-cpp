cmake_minimum_required(VERSION 3.20)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(command_parser VERSION ${PROJECT_VERSION})

FILE(GLOB PROJECT_SOURCES *.h *.cpp)

add_library(async SHARED async.cpp)
add_executable(command_parser main.cpp)

set_target_properties(command_parser PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(command_parser PRIVATE async)

if (MSVC)
    target_compile_options(command_parser PRIVATE
        /W4
    )
else ()
    target_compile_options(command_parser PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS async LIBRARY DESTINATION lib)
install(TARGETS command_parser RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)