// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/shared_string.h>
#include <rethink/ref_string.h>

#include <cstring>

#include <catch.hpp>

using namespace rethink;
using namespace rethink::detail;
using namespace std;

TEST_CASE("foo") {
   void *p = allocate_shared_ctrl(5);
   CHECK(alignof(shared_ctrl) == 4);
   REQUIRE(p != nullptr);
   free_shared_ctrl(p);
}

TEST_CASE("new ctrol block", "[ctrl_block]") {
   size_t start = shared_ctrl::instance_count();
   ref_string r = "foo";
   CHECK(string_size(r) == 3);
   CHECK(strncmp("foo", string_data(r), r.size()) == 0);
   char const* data = new_shared_ctrl(r);
   CHECK(shared_ctrl::instance_count() == start + 1);
   shared_ctrl* ctrl = shared_ctrl_from_data(data);
   REQUIRE(ctrl->ref_count() == 1);
   ctrl->release();
   CHECK(shared_ctrl::instance_count() == start);
}

TEST_CASE("Ctrl block offset", "[ctrl_block]") {
   size_t start = shared_ctrl::instance_count();

   shared_ctrl* ctrl = new shared_ctrl();
   REQUIRE(ctrl->ref_count() == 1);
   CHECK(shared_ctrl::instance_count() == start + 1);

   CHECK(shared_ctrl_from_data(&ctrl->data[0]) == ctrl);

   ctrl->release();
   CHECK(shared_ctrl::instance_count() == start);
}

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
