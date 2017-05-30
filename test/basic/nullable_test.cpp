// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/nullable.h>
#include <rethink/shared_string.h>

#include <catch.hpp>

#include <cstring>
#include <string>

using namespace rethink;
using namespace rethink::detail;
using namespace std;

TEST_CASE("nullable unset", "[nullable] [shared_string]") {
  nullable<shared_string> n;
  CHECK(sizeof(n) == sizeof(shared_string));
  CHECK(n.is_set() == false);
}

TEST_CASE("nullable LHS is unset", "[nullable] [shared_string]") {
  size_t start = ctrl_block::instance_count();
  nullable<shared_string> n;
  CHECK(n.is_set() == false);

  SECTION("copy assign from unset") {
    nullable<shared_string> n2{};
    n = n2;
    CHECK(n.is_set() == false);
  }

  SECTION("copy assign from set") {
    nullable<shared_string> n2{"foo"};
    n = n2;
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("copy assign from T") {
    shared_string s{"foo"};
    n = s;
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("move assign from unset") {
    n = nullable<shared_string>{};
    CHECK(n.is_set() == false);
  }

  SECTION("move assign from set") {
    n = nullable<shared_string>{"foo"};
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("move assign from T") {
    n = shared_string{"foo"};
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  n.unset();
  CHECK(ctrl_block::instance_count() == start);
}

TEST_CASE("nullable LHS is set", "[nullable] [shared_string]") {
  size_t start = ctrl_block::instance_count();
  nullable<shared_string> n{"bar"};
  CHECK(strncmp(n.get().data(), "bar", n.get().size()) == 0);
  CHECK(n.is_set() == true);

  SECTION("copy assign from unset") {
    nullable<shared_string> n2{};
    n = n2;
    CHECK(n.is_set() == false);
  }

  SECTION("copy assign from set") {
    nullable<shared_string> n2{"foo"};
    n = n2;
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("copy assign from T") {
    shared_string s{"foo"};
    n = s;
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("move assign from unset") {
    n = nullable<shared_string>{};
    CHECK(n.is_set() == false);
  }

  SECTION("move assign from set") {
    n = nullable<shared_string>{"foo"};
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("move assign from T") {
    n = shared_string{"foo"};
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  n.unset();
  CHECK(ctrl_block::instance_count() == start);
}

TEST_CASE("nullable<string> unset", "[nullable]") {
  nullable<std::string> n;
  CHECK(sizeof(n) > sizeof(std::string));
  CHECK(n.is_set() == false);
}

TEST_CASE("nullable<string> LHS is unset", "[nullable]") {
  size_t start = ctrl_block::instance_count();
  nullable<std::string> n;
  CHECK(n.is_set() == false);

  SECTION("copy assign from unset") {
    nullable<std::string> n2{};
    n = n2;
    CHECK(n.is_set() == false);
  }

  SECTION("copy assign from set") {
    nullable<std::string> n2{"foo"};
    n = n2;
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("copy assign from T") {
    std::string s{"foo"};
    n = s;
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("move assign from unset") {
    n = nullable<std::string>{};
    CHECK(n.is_set() == false);
  }

  SECTION("move assign from set") {
    n = nullable<std::string>{"foo"};
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("move assign from T") {
    n = std::string{"foo"};
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  n.unset();
  CHECK(ctrl_block::instance_count() == start);
}

TEST_CASE("nullable<string> LHS is set", "[nullable]") {
  size_t start = ctrl_block::instance_count();
  nullable<std::string> n{"bar"};
  CHECK(strncmp(n.get().data(), "bar", n.get().size()) == 0);
  CHECK(n.is_set() == true);

  SECTION("copy assign from unset") {
    nullable<std::string> n2{};
    n = n2;
    CHECK(n.is_set() == false);
  }

  SECTION("copy assign from set") {
    nullable<std::string> n2{"foo"};
    n = n2;
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("copy assign from T") {
    std::string s{"foo"};
    n = s;
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("move assign from unset") {
    n = nullable<std::string>{};
    CHECK(n.is_set() == false);
  }

  SECTION("move assign from set") {
    n = nullable<std::string>{"foo"};
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  SECTION("move assign from T") {
    n = std::string{"foo"};
    CHECK(n.is_set() == true);
    CHECK(strncmp(n.get().data(), "foo", n.get().size()) == 0);
  }

  n.unset();
  CHECK(ctrl_block::instance_count() == start);
}
