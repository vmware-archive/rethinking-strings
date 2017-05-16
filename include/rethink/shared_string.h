// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/api.h>
#include <rethink/detail/ctrl_block.h>
#include <rethink/ref_string.h>

#include <utility>

namespace rethink {

//------------------------------------------------------------------------------

class shared_string {
 public:
  constexpr shared_string() = default;

  shared_string(const shared_string& rhs) : _data(rhs._data) {
    detail::retain_ctrl_block(_data);
  }

  shared_string& operator=(const shared_string& rhs) {
    if (this != &rhs) {
      shared_string tmp(rhs);
      swap(tmp);
    }
    return *this;
  }

  shared_string(shared_string&& rhs) noexcept : _data(rhs.detach()) {}

  shared_string& operator=(shared_string&& rhs) noexcept {
    shared_string tmp(std::move(rhs));
    swap(tmp);
    return *this;
  }

  void swap(shared_string& rhs) noexcept { std::swap(_data, rhs._data); }

  ~shared_string() { detail::release_ctrl_block(_data); }

 public:
  shared_string(ref_string r) : _data(detail::new_ctrl_block(r)){};
  shared_string& operator=(ref_string r) {
    shared_string tmp(r);
    swap(tmp);
    return *this;
  }

  const char* data() const noexcept { return _data; }

  int size() const noexcept { return detail::size_ctrl_block(_data); }

 private:
  const char* detach() noexcept {
    const char* tmp = _data;
    _data = nullptr;
    return tmp;
  }

 private:
  const char* _data{nullptr};
};

//------------------------------------------------------------------------------

inline void swap(shared_string& lhs, shared_string& rhs) { lhs.swap(rhs); }
inline const char* string_data(const shared_string& s) { return s.data(); }
inline int string_size(const shared_string& s) { return s.size(); }

}  // namespace rethink
