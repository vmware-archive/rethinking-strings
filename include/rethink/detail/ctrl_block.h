// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/api.h>

#include <atomic>
#include <cstring>
#include <new>

namespace rethink {
namespace detail {

//------------------------------------------------------------------------------

RETHINK_API void* allocate_ctrl_block(int size);
RETHINK_API void free_ctrl_block(void* p);

//------------------------------------------------------------------------------

class ctrl_block {
 public:
  constexpr ctrl_block() { ++_s_shared_count; }
  ctrl_block(ctrl_block const&) = delete;
  template <class T>
  explicit ctrl_block(const T& t) : len(string_size(t)) {
    char* d = const_cast<char*>(&data[0]);
    std::memcpy(d, string_data(t), len);
    *(d + len) = '\0';
    ++_s_shared_count;
  }
  void retain() noexcept { ++_rc; }
  void release() noexcept {
    if (_rc.fetch_sub(1) == 1) {
      free_ctrl_block(this);
      --_s_shared_count;
    }
  }
  int ref_count() const noexcept { return _rc; }
  static size_t instance_count() noexcept { return _s_shared_count; }

 private:
  static std::atomic<std::size_t> _s_shared_count;

 private:
  std::atomic<int> _rc{1};

 public:
  int const len{0};
  char const data[1]{'\0'};
};

//------------------------------------------------------------------------------

constexpr ptrdiff_t k_ctrl_block_offset{8};

//------------------------------------------------------------------------------

inline ctrl_block* ctrl_block_from_data(const char* d) {
  uintptr_t ctrl = reinterpret_cast<uintptr_t>(d) - k_ctrl_block_offset;
  return reinterpret_cast<ctrl_block*>(ctrl);
}

//------------------------------------------------------------------------------

inline void retain_ctrl_block(const char* data) {
  if (data != nullptr) {
    ctrl_block_from_data(data)->retain();
  }
}

//------------------------------------------------------------------------------

inline void release_ctrl_block(const char* data) {
  if (data != nullptr) {
    ctrl_block_from_data(data)->release();
  }
}

//------------------------------------------------------------------------------

inline int size_ctrl_block(const char* data) {
  return data != nullptr ? ctrl_block_from_data(data)->len : 0;
}

//------------------------------------------------------------------------------

template <class T>
inline char const* new_ctrl_block(const T& t) {
  void* p = allocate_ctrl_block(string_size(t));
  new (p) ctrl_block(t);
  return &reinterpret_cast<ctrl_block*>(p)->data[0];
}

}  // namespace detail
}  // namespace rethink
