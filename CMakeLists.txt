# Copyright (c) 2017 VMware, Inc. All Rights Reserved.

cmake_minimum_required (VERSION 3.8)

project(rethink LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

# Library

add_library(rethink SHARED src/rethink.cpp)
target_include_directories(rethink PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_property(TARGET rethink PROPERTY DEFINE_SYMBOL "EXPORT_RETHINK_API")

install(
    TARGETS rethink
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(
    DIRECTORY "include"
    DESTINATION "."
    FILES_MATCHING
    PATTERN *.h
)

# Tests

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Catch/include)

add_subdirectory(test/basic)

# Add a 'check' target that depends on chap and dumps output on failure. It
# seems like this should be possible without a custom target, but attempts to
# use CTEST_OUTPUT_ON_FAILURE failed, as did attempts set DEPENDS on the tests
# directly.

if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check
        COMMAND ${CMAKE_CTEST_COMMAND}
            --force-new-ctest-process --output-on-failure
            --build-config "$<CONFIGURATION>"
        DEPENDS basic rethink
    )
else()
    add_custom_target(check
        COMMAND ${CMAKE_CTEST_COMMAND}
            --force-new-ctest-process --output-on-failure
        DEPENDS basic rethink
    )
endif()

