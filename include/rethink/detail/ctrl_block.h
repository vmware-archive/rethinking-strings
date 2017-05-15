// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/api.h>

#include <atomic>
#include <cstdlib>
#include <cstring>
#include <new>

#include <stdlib.h>

namespace rethink {
namespace detail {

//------------------------------------------------------------------------------

class shared_ctrl {
 public:
  constexpr shared_ctrl() = default;
  shared_ctrl(shared_ctrl const&) = delete;
  template <class T>
  explicit shared_ctrl(const T& t) : _rc(0), len(string_size(t)) {
    char* d = const_cast<char*>(&data[0]);
    std::memcpy(d, string_data(t), len);
    *(d + len) = '\0';
  }
  void retain() noexcept { ++_rc; }
  void release() noexcept {
    if (_rc.fetch_sub(1) == 1) {
      delete this;
    }
  }

 private:
  std::atomic<int> _rc{1};

 public:
  int const len{0};
  char const data[1]{'\0'};
};

//------------------------------------------------------------------------------

constexpr ptrdiff_t k_shared_ctrl_offset{-8};

//------------------------------------------------------------------------------

inline shared_ctrl* shared_ctrl_from_data(const char* d) {
  uintptr_t ctrl = reinterpret_cast<uintptr_t>(d) + k_shared_ctrl_offset;
  return reinterpret_cast<shared_ctrl*>(ctrl);
}

//------------------------------------------------------------------------------

inline void retain_shared_ctrl(const char* data) {
  if (data != nullptr) {
    shared_ctrl_from_data(data)->retain();
  }
}

//------------------------------------------------------------------------------

inline void release_shared_ctrl(const char* data) {
  if (data != nullptr) {
    shared_ctrl_from_data(data)->release();
  }
}

//------------------------------------------------------------------------------

inline int size_shared_ctrl(const char* data) {
  return data != nullptr ? shared_ctrl_from_data(data)->len : 0;
}

//------------------------------------------------------------------------------

RETHINK_API void* allocate_shared_ctrl(int size);

//------------------------------------------------------------------------------

template <class T>
inline char const* new_shared_ctrl(const T& t) {
  void* p = allocate_shared_ctrl(string_size(t));
  new (p) shared_ctrl(t);
  return &reinterpret_cast<shared_ctrl*>(p)->data[0];
}

}  // namespace detail
}  // namespace rethink
