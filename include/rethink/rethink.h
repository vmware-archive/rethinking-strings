// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/api.h>

#include <atomic>

namespace rethink {

//------------------------------------------------------------------------------

class ref_string {
 public:
  template <int N>
  constexpr ref_string(char const (&a)[N]) : _size(N), _data(&a[0]) {}
  constexpr ref_string() : _size(0), _data(nullptr) {}
  void swap(ref_string& rhs) noexcept {
    using std::swap;
    swap(_data, rhs._data);
    swap(_size, rhs._size);
  }

  const char* data() const { return _data; }
  int size() const { return _size; }

 private:
  int _size;
  char const* _data;  // TODO Need non-const ref_string.
};

//------------------------------------------------------------------------------

inline void swap(ref_string& lhs, ref_string& rhs) { lhs.swap(rhs); }
inline const char* data(ref_string const& s) { return s.data(); }
inline int size(ref_string const& s) { return s.size(); }

namespace detail {

//------------------------------------------------------------------------------

class shared_ctrl {
 public:
  constexpr shared_ctrl() {}
  void retain() noexcept { ++_rc; }
  void release() noexcept {
    if (_rc.fetch_sub(1) == 0) {
      delete this;
    }
  }

 private:
  std::atomic<int> _rc = 0;  // TODO choose non-default ordering?

 public:
  int cap = 0;
  int len = 0;
  const char data[1] = {'\0'};
};

//------------------------------------------------------------------------------

constexpr ptrdiff_t k_shared_ctrl_offset = -12;
RETHINK_API constexpr shared_ctrl s_empty_shared_ctrl;

//------------------------------------------------------------------------------

inline shared_ctrl* shared_ctrl_from_data(const char* d) {
  uintptr_t ctrl = reinterpret_cast<uintptr_t>(d) + k_shared_ctrl_offset;
  return reinterpret_cast<shared_ctrl*>(ctrl);
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
inline const char* data(const shared_string& s) { return s.data(); }
inline int size(const shared_string& s) { return s.size(); }

}  // namespace rethink
