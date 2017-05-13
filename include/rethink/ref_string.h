// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <utility>

namespace rethink {

//------------------------------------------------------------------------------

class ref_string {
 public:
  template <int N>
  constexpr ref_string(char const (&a)[N]) : _size(N), _data(&a[0]) {}
  template <class T>
  constexpr ref_string(const T& t)
      : _size(string_size(t)), _data(string_data(t)) {}
  constexpr ref_string() noexcept : _size(0), _data(nullptr) {}
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
