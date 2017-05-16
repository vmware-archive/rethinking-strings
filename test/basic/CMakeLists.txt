# Copyright (c) 2017 VMware, Inc. All Rights Reserved.

add_executable(basic
               basic.cpp
               ctrl_block_test.cpp
               ref_string_test.cpp
               shared_string_test.cpp
               unique_string_test.cpp
)
target_link_libraries(basic Catch rethink)

add_test(
    NAME BasicTest
    COMMAND basic
)
