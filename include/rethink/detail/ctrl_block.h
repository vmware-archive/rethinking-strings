// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/api.h>

#include <atomic>
#include <cstring>
#include <new>

namespace rethink {
namespace detail {

//------------------------------------------------------------------------------

class ctrl_block {
 public:
  constexpr ctrl_block() { ++_s_shared_count; }
  ctrl_block(ctrl_block const&) = delete;
  template <class T>
  explicit ctrl_block(const T& t) : _size(string_size(t)) {
    std::memcpy(_data, string_data(t), _size);
    *(_data + _size) = '\0';
    ++_s_shared_count;
  }
  void retain() const noexcept { ++_rc; }
  void release() const noexcept {
    if (_rc.fetch_sub(1) == 1) {
      this->~ctrl_block();
      free(const_cast<ctrl_block*>(this));
      --_s_shared_count;
    }
  }

 public:
  const char* data() const noexcept { return _data; }
  char* data() noexcept { return _data; }
  int size() const noexcept { return _size; }
  int ref_count() const noexcept { return _rc; }
  static size_t instance_count() noexcept { return _s_shared_count; }

 private:
  static std::atomic<std::size_t> _s_shared_count;

 private:
  mutable std::atomic<int> _rc{1};
  int const _size{0};
  char _data[1]{'\0'};
};

//------------------------------------------------------------------------------

template <class T>
inline char* new_ctrl_block(const T& t) {
  void* p = malloc(sizeof(ctrl_block) + string_size(t));
  new (p) ctrl_block(t);
  return reinterpret_cast<ctrl_block*>(p)->data();
}

//------------------------------------------------------------------------------

constexpr ptrdiff_t k_ctrl_block_offset{8};

//------------------------------------------------------------------------------

inline ctrl_block* ctrl_block_from_data(char* d) {
  uintptr_t ctrl = reinterpret_cast<uintptr_t>(d) - k_ctrl_block_offset;
  return reinterpret_cast<ctrl_block*>(ctrl);
}

inline const ctrl_block* ctrl_block_from_data(const char* d) {
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
  return data != nullptr ? ctrl_block_from_data(data)->size() : 0;
}

}  // namespace detail
}  // namespace rethink
