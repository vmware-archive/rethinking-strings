// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/shared_string.h>
#include <rethink/ref_string.h>

#include <cstring>

#include <catch.hpp>

using namespace rethink;
using namespace rethink::detail;
using namespace std;

TEST_CASE("foo") {
   void *p = allocate_ctrl_block(5);
   CHECK(alignof(ctrl_block) == 4);
   REQUIRE(p != nullptr);
   free_ctrl_block(p);
}

TEST_CASE("new ctrol block", "[ctrl_block]") {
   size_t start = ctrl_block::instance_count();
   ref_string r = "foo";
   CHECK(string_size(r) == 3);
   CHECK(strncmp("foo", string_data(r), r.size()) == 0);
   char const* data = new_ctrl_block(r);
   CHECK(ctrl_block::instance_count() == start + 1);
   ctrl_block* ctrl = ctrl_block_from_data(data);
   REQUIRE(ctrl->ref_count() == 1);
   ctrl->release();
   CHECK(ctrl_block::instance_count() == start);
}

TEST_CASE("Ctrl block offset", "[ctrl_block]") {
   size_t start = ctrl_block::instance_count();

   ctrl_block* ctrl = new ctrl_block();
   REQUIRE(ctrl->ref_count() == 1);
   CHECK(ctrl_block::instance_count() == start + 1);

   CHECK(ctrl_block_from_data(&ctrl->data[0]) == ctrl);

   ctrl->release();
   CHECK(ctrl_block::instance_count() == start);
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
