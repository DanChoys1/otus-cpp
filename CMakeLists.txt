cmake_minimum_required(VERSION 3.20)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VERSION})

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS *.h *.cpp)

find_package(Boost REQUIRED COMPONENTS filesystem program_options)

add_executable(bayan main.cpp bayan.cpp)

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(bayan PRIVATE Boost::filesystem Boost::program_options Boost::system)

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)