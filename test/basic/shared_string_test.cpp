// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/shared_string.h>

#include <cstring>

#include <catch.hpp>

using namespace rethink;
using namespace std;

TEST_CASE("Empty shared_string is empty", "[ref_string]") {
  shared_string s;
  CHECK(string_size(s) == 0);
}

