// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/ref_string.h>
#include <rethink/unique_string.h>

#include <catch.hpp>

using namespace rethink;
using namespace rethink::detail;
using namespace std;

TEST_CASE("Empty unique string is empty", "[unique_string]") {
  unique_string s;
  CHECK(string_size(s) == 0);
}

TEST_CASE("unique_string is_transferable", "[unique_string]") {
  CHECK(rethink::is_transferable_v<unique_string &&> == true);

  CHECK(rethink::is_transferable_v<unique_string> == false);
  CHECK(rethink::is_transferable_v<unique_string &> == false);
  CHECK(rethink::is_transferable_v<unique_string const &> == false);
  CHECK(rethink::is_transferable_v<unique_string *> == false);
}

TEST_CASE("unique_string is_shareable", "[unique_string]") {
  CHECK(rethink::is_shareable_v<unique_string &&> == false);
  CHECK(rethink::is_shareable_v<unique_string> == false);
  CHECK(rethink::is_shareable_v<unique_string &> == false);
  CHECK(rethink::is_shareable_v<unique_string const &> == false);
  CHECK(rethink::is_shareable_v<unique_string *> == false);
}

TEST_CASE("Unique string value ops", "[unique_string]") {
  size_t start = ctrl_block::instance_count();

  SECTION("Unique strings init from ref_string") {
    ref_string r = "foo";
    unique_string s(r);
    CHECK(string_size(s) == 3);
    CHECK(s.size() == 3);
    CHECK(strncmp(string_data(s), r.data(), s.size()) == 0);
    CHECK(strncmp(s.data(), r.data(), s.size()) == 0);
  }

  SECTION("Unique strings assigned from ref_string") {
    unique_string s;
    ref_string r = "foo";
    s = r;
    CHECK(string_size(s) == 3);
    CHECK(s.size() == 3);
    CHECK(strncmp(string_data(s), r.data(), s.size()) == 0);
    CHECK(strncmp(s.data(), r.data(), s.size()) == 0);
  }

  SECTION("Unique string copied") {
    unique_string from = ref_string("foo");
    unique_string s(from);
    CHECK(from.size() == 3);
    CHECK(s.size() == 3);
    CHECK(strncmp(s.data(), "foo", s.size()) == 0);
  }

  SECTION("Unique string assigned") {
    unique_string from = ref_string("foo");
    unique_string s;
    s = from;
    CHECK(from.size() == 3);
    CHECK(s.size() == 3);
    CHECK(strncmp(s.data(), "foo", s.size()) == 0);
  }

  SECTION("Unique string move copied") {
    unique_string from = ref_string("foo");
    unique_string s(move(from));
    CHECK(from.size() == 0);
    CHECK(s.size() == 3);
    CHECK(strncmp(s.data(), "foo", s.size()) == 0);
  }

  SECTION("Unique string move assigned") {
    unique_string from = ref_string("foo");
    unique_string s;
    s = move(from);
    CHECK(from.size() == 0);
    CHECK(s.size() == 3);
    CHECK(strncmp(s.data(), "foo", s.size()) == 0);
  }

  CHECK(ctrl_block::instance_count() == start);
}

TEST_CASE("Unique strings do not share storage", "[unique_string]") {
  size_t start = ctrl_block::instance_count();
  {
    unique_string one = ref_string("foo");
    CHECK(ctrl_block::instance_count() == start + 1);
    {
      unique_string two = one;
      CHECK(ctrl_block::instance_count() == start + 2);
    }
    CHECK(ctrl_block::instance_count() == start + 1);
  }
  CHECK(ctrl_block::instance_count() == start);
}
#if 0
#endif
