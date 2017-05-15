// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/rethink.h>

#include <utility>

namespace rethink {

//------------------------------------------------------------------------------

class ref_string {
 public:
  constexpr ref_string() noexcept : _size(0), _data(nullptr) {}
  template <class T>
  constexpr ref_string(const T& t)
      : _size(string_size(t)), _data(string_data(t)) {}
  template <class T>
  ref_string& operator=(const T& t) {
    _size = string_size(t);
    _data = string_data(t);
    return *this;
  }
  void swap(ref_string& rhs) noexcept {
    std::swap(_data, rhs._data);
    std::swap(_size, rhs._size);
  }

  constexpr char const* data() const noexcept { return _data; }
  constexpr int size() const noexcept { return _size; }

 private:
  int _size;
  char const* _data;
};

//------------------------------------------------------------------------------

inline void swap(ref_string& lhs, ref_string& rhs) { lhs.swap(rhs); }
inline const char* string_data(ref_string const& s) { return s.data(); }
inline int string_size(ref_string const& s) { return s.size(); }

}  // namespace rethink
