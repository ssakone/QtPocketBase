cmake_minimum_required(VERSION 3.14)

set(This PocketBaseTest)



find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main


    Qt6::Core
)

add_test(
    NAME ${This}
    COMMAND ${This}
)

target_include_directories(PocketBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

if (MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()