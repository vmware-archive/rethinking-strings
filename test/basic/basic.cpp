// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/ref_string.h>
#include <rethink/shared_string.h>

#include <cstring>

using namespace rethink;
using namespace rethink::detail;
using namespace std;

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do
                           // this in one cpp file
#include "catch.hpp"

TEST_CASE("k_shared_ctrl_offset is good", "[shared_string]") {
  shared_ctrl ctrl;
  uintptr_t data = reinterpret_cast<uintptr_t>(&ctrl.data[0]);
  ptrdiff_t offset = reinterpret_cast<uintptr_t>(&ctrl) - data;
  REQUIRE(offset == k_shared_ctrl_offset);
}

TEST_CASE("The empty shared_string is empty", "[shared_string]") {
  shared_string s;
  REQUIRE(string_size(s) == 0);
}
