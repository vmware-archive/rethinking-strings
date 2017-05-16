// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/shared_string.h>

namespace rethink {
namespace detail {

//------------------------------------------------------------------------------

std::atomic<std::size_t> ctrl_block::_s_shared_count;

}  // namespace detail
}  // namespace rethink
