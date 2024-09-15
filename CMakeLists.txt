cmake_minimum_required(VERSION 3.20)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

option(WITH_GOOGLE_TEST "Whether to build Google test" ON)

add_executable(ip_filter main.cpp ip_filter.h ip_filter.cpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(WITH_GOOGLE_TEST)
    find_package(GTest REQUIRED)
    add_executable(gtest_ip_filter gtest_ip_filter.cpp gtest_data.h ip_filter.h ip_filter.cpp)
    target_link_libraries(gtest_ip_filter GTest::gtest_main)
endif()

if (MSVC)
    target_compile_options(ip_filter PRIVATE
        /W4
    )
    if(WITH_GOOGLE_TEST)
        target_compile_options(gtest_ip_filter PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_GOOGLE_TEST)
        target_compile_options(gtest_ip_filter PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_GOOGLE_TEST)
    enable_testing()
    add_test(gtest_ip_filter gtest_ip_filter)
endif()
