// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/shared_string.h>

namespace rethink {
namespace detail {

//------------------------------------------------------------------------------

std::atomic<std::size_t> shared_ctrl::_s_shared_count;

//------------------------------------------------------------------------------

void free_shared_ctrl(void* p) { free(p); }

//------------------------------------------------------------------------------

void* allocate_shared_ctrl(int size) {
  void* p = malloc(sizeof(shared_ctrl) + size);
  // TODO assert alignment.
  return p;
}

}  // namespace detail
}  // namespace rethink
