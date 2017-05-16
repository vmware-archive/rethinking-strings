// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/ref_string.h>
#include <rethink/shared_string.h>

#include <catch.hpp>

using namespace rethink;
using namespace rethink::detail;
using namespace std;

TEST_CASE("Empty shared string is empty", "[ref_string]") {
  shared_string s;
  CHECK(string_size(s) == 0);
}

TEST_CASE("Shared strings init from ref_string", "[ref_string]") {
  ref_string r = "foo";
  shared_string s(r);
  CHECK(string_size(s) == 3);
  CHECK(s.size() == 3);
  CHECK(strncmp(string_data(s), r.data(), s.size()) == 0);
  CHECK(strncmp(s.data(), r.data(), s.size()) == 0);
}
