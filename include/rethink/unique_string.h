// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/api.h>
#include <rethink/ctrl_block.h>

#include <utility>

namespace rethink {

//------------------------------------------------------------------------------

class unique_string {
 public:
  constexpr shared_string() = default;

  unique_string(const unique_string& rhs) : unique_string(ref_string(rhs)) {}

  unique_string& operator=(const unique_string& rhs) {
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
    if (_data != nullptr) {
      ctrl()->release();
    }
  }

 public:
  explicit unique_string(ref_string r) : _data(detail::new_ctrl_block(r)){};

  unique_string& operator=(const ref_string& rhs) {
    unique_string tmp(rhs);
    swap(tmp);
    return *this;
  }

  const char* data() const { return _cap == 0 ? this : _data; }

  char* data() { return _cap == 0 ? this : _data; }

  int size() const { return ctrl()->len; }

 private:
  detail::ctrl_block* ctrl() const noexcept {
    return detail::ctrl_block_from_data(_data);
  }

  const char* detach() noexcept {
    const char* tmp = _data;
    _data = nullptr;
    return tmp;
  }

 private:
  char* _data{nullptr};
};

//------------------------------------------------------------------------------

inline void swap(unique_string& lhs, unique_string& rhs) { lhs.swap(rhs); }

inline const char* string_data(const unique_string& s) { return s.data(); }

inline int string_size(const unique_string& s) { return s.size(); }

}  // namespace rethink
