cmake_minimum_required(VERSION 3.20)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bulk_server VERSION ${PROJECT_VERSION})

FILE(GLOB PROJECT_SOURCES *.h *.cpp)

add_executable(bulk_server ${PROJECT_SOURCES})

set_target_properties(bulk_server PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

find_package(Boost REQUIRED COMPONENTS system)

target_include_directories(bulk_server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(bulk_server PRIVATE ${Boost_LIBRARIES})

if (MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)