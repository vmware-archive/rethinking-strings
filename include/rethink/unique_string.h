// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/api.h>
#include <rethink/detail/ctrl_block.h>

#include <utility>

namespace rethink {

//------------------------------------------------------------------------------

class unique_string {
 public:
  unique_string() = default;

  unique_string(unique_string const& rhs) : unique_string(ref_string(rhs)) {}

  unique_string& operator=(unique_string const& rhs) {
    return operator=(ref_string(rhs));
  }

  unique_string(unique_string&& rhs) noexcept : _data(rhs.detach()) {}

  unique_string& operator=(unique_string&& rhs) noexcept {
    if (&rhs != this) {
      _data = rhs.detach();
    }
    return *this;
  }

  void swap(unique_string& rhs) noexcept { std::swap(_data, rhs._data); }

  ~unique_string() {
    detail::release_ctrl_block(_data);
  }

 public:
  unique_string(ref_string r) : _data(detail::new_ctrl_block(r)){};

  unique_string& operator=(ref_string r) {
    unique_string tmp(r);
    swap(tmp);
    return *this;
  }

  char const* data() const noexcept { return _data; }

  char* data() noexcept { return _data; }

  int size() const noexcept { return detail::size_ctrl_block(_data); }

 private:
  char* detach() noexcept {
    char* tmp = _data;
    _data = nullptr;
    return tmp;
  }

 private:
  char* _data{nullptr};
};

//------------------------------------------------------------------------------

inline void swap(unique_string& lhs, unique_string& rhs) { lhs.swap(rhs); }

inline char const* string_data(unique_string const& s) { return s.data(); }

inline int string_size(unique_string const& s) { return s.size(); }

}  // namespace rethink
