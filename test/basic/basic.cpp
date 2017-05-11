// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/rethink.h>

#include <cstring>

using namespace rethink;
using namespace rethink::detail;
using namespace std;

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do
                           // this in one cpp file
#include "catch.hpp"

TEST_CASE("Empty ref_string is empty", "[ref_string]") {
  ref_string s;
  REQUIRE(size(s) == 0);
}

TEST_CASE("k_shared_ctrl_offset is good", "[shared_string]") {
  shared_ctrl ctrl;
  uintptr_t data = reinterpret_cast<uintptr_t>(&ctrl.data[0]);
  ptrdiff_t offset = reinterpret_cast<uintptr_t>(&ctrl) - data;
  REQUIRE(offset == k_shared_ctrl_offset);
}

TEST_CASE("The empty shared_string always has data", "[shared_string]") {
  shared_string s;
  REQUIRE(data(s) != nullptr);
  REQUIRE(strlen(data(s)) == 0);
  REQUIRE(size(s) == 0);
}

TEST_CASE("shared_string copies correctly", "[shared_string]") {
  shared_string s;
  REQUIRE(data(s) != nullptr);
  REQUIRE(strlen(data(s)) == 0);
  REQUIRE(size(s) == 0);
}
