// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/shared_string.h>

namespace rethink {
namespace detail {

//------------------------------------------------------------------------------

std::atomic<std::size_t> ctrl_block::_s_shared_count;

//------------------------------------------------------------------------------

void free_ctrl_block(void* p) { free(p); }

//------------------------------------------------------------------------------

void* allocate_ctrl_block(int size) {
  void* p = malloc(sizeof(ctrl_block) + size);
  // TODO assert alignment.
  return p;
}

}  // namespace detail
}  // namespace rethink
