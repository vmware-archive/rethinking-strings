# Copyright (c) 2017 VMware, Inc. All Rights Reserved.

add_executable(std std.cpp)
target_link_libraries(std rethink)

add_executable(rth rth.cpp)
target_link_libraries(rth rethink)

add_test(
    NAME SizeTest
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/diffsize
            $<TARGET_FILE:rth>
            $<TARGET_FILE:std>
)
