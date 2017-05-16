// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/detail/ctrl_block.h>
#include <rethink/ref_string.h>

#include <catch.hpp>

using namespace rethink;
using namespace rethink::detail;
using namespace std;

TEST_CASE("new ctrol block", "[ctrl_block]") {
  size_t start = ctrl_block::instance_count();
  ref_string r = "foo";
  char const* data = new_ctrl_block(r);
  CHECK(ctrl_block::instance_count() == start + 1);
  const ctrl_block* ctrl = ctrl_block_from_data(data);
  REQUIRE(ctrl->ref_count() == 1);
  ctrl->release();
  CHECK(ctrl_block::instance_count() == start);
}

TEST_CASE("Ctrl block offset", "[ctrl_block]") {
  size_t start = ctrl_block::instance_count();

  ctrl_block* ctrl = new ctrl_block();
  REQUIRE(ctrl->ref_count() == 1);
  CHECK(ctrl_block::instance_count() == start + 1);

  CHECK(ctrl_block_from_data(ctrl->data()) == ctrl);

  ctrl->release();
  CHECK(ctrl_block::instance_count() == start);
}
