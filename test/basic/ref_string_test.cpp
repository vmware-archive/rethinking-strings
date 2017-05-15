// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/ref_string.h>

#include <cstring>
#include <string_view>

#include <catch.hpp>

using namespace rethink;
using namespace std;

TEST_CASE("Empty ref_string is empty", "[ref_string]") {
  ref_string s;
  CHECK(string_size(s) == 0);
}

TEST_CASE("Construct ref_string from char[]", "[ref_string]") {
  constexpr char k_foo[] = "foo";
  ref_string s(k_foo);
  CHECK(string_size(s) == strlen(k_foo));
  CHECK(string_size(s) == string_view(k_foo).size());
  CHECK(string_data(s) == k_foo);
}

TEST_CASE("Construct ref_string from char[] with embedded null",
          "[ref_string]") {
  constexpr char k_foo[] = "foo\0bar";
  ref_string s(k_foo);
  CHECK(string_size(s) == sizeof(k_foo) - 1);
  CHECK(string_data(s) == k_foo);
}

struct ref_from_foo {
  static constexpr char k_foo[] = "foo";
  friend const char* string_data(ref_from_foo) { return ref_from_foo::k_foo; }
  friend int string_size(ref_from_foo) { return strlen(ref_from_foo::k_foo); }
};

TEST_CASE("Construct ref_string from type with traits", "[ref_string]") {
  ref_string s((ref_from_foo()));
  CHECK(string_size(s) == strlen(ref_from_foo::k_foo));
  CHECK(string_data(s) == ref_from_foo::k_foo);
}

TEST_CASE("ref_string swap works as expected", "[ref_string]") {
  ref_string s("foo");
  ref_string t("bar");
  swap(s, t);
  CHECK(strncmp(s.data(), "bar", s.size()) == 0);
  CHECK(strncmp(t.data(), "foo", t.size()) == 0);
}
