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
  explicit shared_ctrl(const T& t) : len(string_size(t)), _rc(0) {
    char* d = const_cast<char*>(&data[0]);
    std::memcpy(d, string_data(t), len);
    *(d + len) = '\0';
  }
  void retain() noexcept { ++_rc; }
  void release() noexcept {
    if (_rc.fetch_sub(1) == 0) {
      delete this;
    }
  }

 public:
  int const len{0};

 private:
  std::atomic<int> _rc{0};

 public:
  char const data[1]{'\0'};
};

//------------------------------------------------------------------------------

constexpr ptrdiff_t k_shared_ctrl_offset = -8;
RETHINK_API constexpr shared_ctrl s_empty_shared_ctrl;

//------------------------------------------------------------------------------

inline shared_ctrl* shared_ctrl_from_data(const char* d) {
  uintptr_t ctrl = reinterpret_cast<uintptr_t>(d) + k_shared_ctrl_offset;
  return reinterpret_cast<shared_ctrl*>(ctrl);
}

//------------------------------------------------------------------------------

template <class T>
inline char const* new_shared_ctrl(const T& t) {
  void* p;
  posix_memalign(&p, alignof(shared_ctrl),
                 sizeof(shared_ctrl) + string_size(t));
  new (p) shared_ctrl(t);
  return &reinterpret_cast<shared_ctrl*>(p)->data[0];
}

}  // namespace detail

//------------------------------------------------------------------------------

class shared_string {
 public:
  constexpr shared_string() : _data(&detail::s_empty_shared_ctrl.data[0]) {
    ctrl()->retain();
  }
  shared_string(const shared_string& rhs) : _data(rhs._data) {
    ctrl()->retain();
  }
  shared_string& operator=(const shared_string& rhs) {
    if (this != &rhs) {
      shared_string tmp(rhs);
      swap(tmp);
    }
    return *this;
  }
  shared_string(shared_string&& rhs) noexcept : _data(rhs.detach()) {}
  shared_string& operator=(shared_string&& rhs) {
    ctrl()->release();
    rhs.detach();
    return *this;
  }
  void swap(shared_string& rhs) noexcept { std::swap(_data, rhs._data); }
  ~shared_string() { ctrl()->release(); }

 public:
  explicit shared_string(ref_string r) : _data(detail::new_shared_ctrl(r)){};

  const char* data() const { return _data; }
  int size() const { return ctrl()->len; }

 private:
  detail::shared_ctrl* ctrl() const noexcept {
    return detail::shared_ctrl_from_data(_data);
  }
  const char* detach() noexcept {
    const char* tmp = _data;
    _data = nullptr;
    return tmp;
  }

 private:
  const char* _data;
};

//------------------------------------------------------------------------------

inline void swap(shared_string& lhs, shared_string& rhs) { lhs.swap(rhs); }
inline const char* string_data(const shared_string& s) { return s.data(); }
inline int string_size(const shared_string& s) { return s.size(); }

}  // namespace rethink
