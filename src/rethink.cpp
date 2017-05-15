
// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/shared_string.h>

namespace rethink {
namespace detail {

//------------------------------------------------------------------------------

void* allocate_shared_ctrl(int size) {
  void* p;
  posix_memalign(&p, alignof(shared_ctrl), sizeof(shared_ctrl) + size);
  // TODO check result of posix_memalign.
  return p;
}

} // namespace detail
} // namespace rethink


