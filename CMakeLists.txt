cmake_minimum_required(VERSION 3.20)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(command_parser VERSION ${PROJECT_VESRION})

FILE(GLOB PROJECT_SOURCES *.h *.cpp)

add_executable(command_parser ${PROJECT_SOURCES})

set_target_properties(command_parser PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(command_parser PRIVATE
        /W4
    )
else ()
    target_compile_options(command_parser PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS command_parser RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)