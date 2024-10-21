cmake_minimum_required(VERSION 3.20)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(editor VERSION ${PROJECT_VESRION})

include_directories(geometry/)
include_directories(primitives/)
include_directories(helpers/)
include_directories(view/)
include_directories(model/)

FILE(GLOB PROJECT_SOURCES *.h *.cpp)

add_executable(editor ${PROJECT_SOURCES})

set_target_properties(editor PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(editor PRIVATE
        /W4
    )
else ()
    target_compile_options(editor PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS editor RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)