// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/api.h>
#include <rethink/detail/ctrl_block.h>

#include <utility>

namespace rethink {

//------------------------------------------------------------------------------

class unique_string;
void swap(unique_string& lhs, unique_string& rhs);
char const* string_data(unique_string const& s);
char* string_data(unique_string& s);
int string_size(unique_string const& s);

//------------------------------------------------------------------------------

class unique_string {
 public:
  unique_string() : _data(nullptr) {}

  void swap(unique_string& rhs) noexcept { std::swap(_data, rhs._data); }

  ~unique_string() { detail::release_ctrl_block(_data); }

 public:
  template <class T>
  unique_string(T&& rhs) {
    if
      constexpr(is_transferable_v<decltype(rhs)>) {
        _data = std::forward<T>(rhs).transfer();
      }
    else {
      _data = detail::new_ctrl_block(rhs);
    }
  }

  template <class T>
  unique_string& operator=(T&& rhs) {
    unique_string tmp(std::forward<T>(rhs));
    swap(tmp);
    return *this;
  }

  char const* data() const noexcept { return _data; }

  char* data() noexcept { return _data; }

  int size() const noexcept { return detail::size_ctrl_block(_data); }

  char* transfer() && noexcept {
    char* tmp = _data;
    _data = nullptr;
    return tmp;
  }

 private:
  char* _data;
};

//------------------------------------------------------------------------------

inline void swap(unique_string& lhs, unique_string& rhs) { lhs.swap(rhs); }

inline char const* string_data(unique_string const& s) { return s.data(); }

inline char* string_data(unique_string& s) { return s.data(); }

inline int string_size(unique_string const& s) { return s.size(); }

template <>
struct is_transferable<unique_string&&> : std::true_type {};

}  // namespace rethink
