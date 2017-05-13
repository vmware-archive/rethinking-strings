# Copyright (c) 2017 VMware, Inc. All Rights Reserved.

add_executable(basic basic.cpp)
target_link_libraries(basic Catch rethink)

add_test(
    NAME BasicTest
    COMMAND basic
)
