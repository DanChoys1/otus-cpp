cmake_minimum_required(VERSION 3.20)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(orm VERSION ${PROJECT_VERSION})

FILE(GLOB PROJECT_SOURCES *.h *.cpp orm/*.h orm/*.cpp)

# find_package(SQLite3 REQUIRED)

add_executable(orm ${PROJECT_SOURCES})

set_target_properties(orm PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(orm PRIVATE orm)
# target_link_libraries(orm PRIVATE SQLite::SQLite3)

if (MSVC)
    target_compile_options(orm PRIVATE
        /W4 /Zc:preprocessor
    )
else ()
    target_compile_options(orm PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS orm RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)