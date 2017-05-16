// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/ref_string.h>
#include <rethink/shared_string.h>

#include <catch.hpp>

using namespace rethink;
using namespace rethink::detail;
using namespace std;

TEST_CASE("Empty shared string is empty", "[shared_string]") {
  shared_string s;
  CHECK(string_size(s) == 0);
}

TEST_CASE("Shared string value ops", "[shared_string]") {
  size_t start = ctrl_block::instance_count();

  SECTION("Shared strings init from ref_string") {
    ref_string r = "foo";
    shared_string s(r);
    CHECK(string_size(s) == 3);
    CHECK(s.size() == 3);
    CHECK(strncmp(string_data(s), r.data(), s.size()) == 0);
    CHECK(strncmp(s.data(), r.data(), s.size()) == 0);
    CHECK(ctrl_block::instance_count() == start + 1);
  }

  SECTION("Shared strings assigned from ref_string") {
    shared_string s;
    ref_string r = "foo";
    s = r;
    CHECK(string_size(s) == 3);
    CHECK(s.size() == 3);
    CHECK(strncmp(string_data(s), r.data(), s.size()) == 0);
    CHECK(strncmp(s.data(), r.data(), s.size()) == 0);
    CHECK(ctrl_block::instance_count() == start + 1);
  }

  SECTION("Shared string copied") {
    shared_string from = ref_string("foo");
    shared_string s(from);
    CHECK(from.size() == 3);
    CHECK(s.size() == 3);
    CHECK(strncmp(s.data(), "foo", s.size()) == 0);
    CHECK(ctrl_block::instance_count() == start + 1);
  }

  SECTION("Shared string assigned") {
    shared_string from = ref_string("foo");
    shared_string s;
    s = from;
    CHECK(from.size() == 3);
    CHECK(s.size() == 3);
    CHECK(strncmp(s.data(), "foo", s.size()) == 0);
    CHECK(ctrl_block::instance_count() == start + 1);
  }

  SECTION("Shared string move copied") {
    shared_string from = ref_string("foo");
    shared_string s(move(from));
    CHECK(from.size() == 0);
    CHECK(s.size() == 3);
    CHECK(strncmp(s.data(), "foo", s.size()) == 0);
    CHECK(ctrl_block::instance_count() == start + 1);
  }

  SECTION("Shared string move assigned") {
    shared_string from = ref_string("foo");
    shared_string s;
    s = move(from);
    CHECK(from.size() == 0);
    CHECK(s.size() == 3);
    CHECK(strncmp(s.data(), "foo", s.size()) == 0);
    CHECK(ctrl_block::instance_count() == start + 1);
  }

  CHECK(ctrl_block::instance_count() == start);
}

TEST_CASE("Shared string share storage", "[shared_string]") {
  size_t start = ctrl_block::instance_count();
  {
      shared_string one = ref_string("foo");
      CHECK(ctrl_block::instance_count() == start + 1);
      {
        shared_string two = one;
        CHECK(ctrl_block::instance_count() == start + 1);
      }
      CHECK(ctrl_block::instance_count() == start + 1);
  }
  CHECK(ctrl_block::instance_count() == start);
}
