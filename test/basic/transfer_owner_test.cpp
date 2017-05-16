// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/ref_string.h>
#include <rethink/shared_string.h>
#include <rethink/unique_string.h>

#include <catch.hpp>

using namespace rethink;
using namespace rethink::detail;
using namespace std;

static int total_control_blocks() { return ctrl_block::instance_count(); }

template <class T>
static int reference_count(T const& t) {
  return ctrl_block_from_data(t.data())->ref_count();
}

TEST_CASE("Transfer from shared to unique", "[unique_string] [shared_string]") {
  size_t start = total_control_blocks();

  shared_string s = ref_string("foo");

  CHECK(reference_count(s) == 1);
  CHECK(total_control_blocks() == start + 1);

  unique_string u(move(s));

  CHECK(total_control_blocks() == start + 1);
  CHECK(s.size() == 0);
  CHECK(u.size() == 3);
}

TEST_CASE("Transfer from doubly shared to unique",
          "[unique_string] [shared_string]") {
  size_t start = ctrl_block::instance_count();
  shared_string s = ref_string("foo");
  shared_string c = s;

  CHECK(reference_count(s) == 2);
  CHECK(total_control_blocks() == start + 1);

  unique_string u(move(s));

  CHECK(total_control_blocks() == start + 2);
  CHECK(s.size() == 0);
  CHECK(u.size() == 3);
}

TEST_CASE("Transfer from unique to shared", "[unique_string] [shared_string]") {
  size_t start = ctrl_block::instance_count();
  unique_string u = ref_string("foo");

  CHECK(total_control_blocks() == start + 1);

  shared_string s(move(u));

  CHECK(total_control_blocks() == start + 1);
  CHECK(u.size() == 0);
  CHECK(s.size() == 3);
}
