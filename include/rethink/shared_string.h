// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/api.h>
#include <rethink/detail/ctrl_block.h>
#include <rethink/ref_string.h>

#include <utility>

namespace rethink {

//------------------------------------------------------------------------------

class shared_string;
void swap(shared_string& lhs, shared_string& rhs);
char const* string_data(shared_string const& s);
int string_size(shared_string const& s);

//------------------------------------------------------------------------------

class shared_string {
 public:
  shared_string() : _data(nullptr) {}

  void swap(shared_string& rhs) noexcept { std::swap(_data, rhs._data); }

  ~shared_string() { detail::release_ctrl_block(_data); }

 public:
  template <class T>
  shared_string(T&& rhs) {
    if constexpr (is_transferable_v<decltype(rhs)>) {
      _data = std::forward<T>(rhs).transfer();
    } else if constexpr (is_shareable_v<decltype(rhs)>) {
      _data = std::forward<T>(rhs).share();
    } else {
      _data = detail::new_ctrl_block(rhs);
    }
  }

  template <class T>
  shared_string& operator=(T&& rhs) {
    shared_string tmp(std::forward<T>(rhs));
    swap(tmp);
    return *this;
  }

  char const* data() const noexcept { return _data; }

  int size() const noexcept { return detail::size_ctrl_block(_data); }

  char* transfer() && {
    if (_data == nullptr) {
      return nullptr;
    }
    if (detail::ctrl_block_from_data(_data)->ref_count() == 1) {
      char* tmp = const_cast<char*>(_data);
      _data = nullptr;
      return tmp;
    }
    return detail::new_ctrl_block(*this);
  }

  char const* share() const {
    detail::retain_ctrl_block(_data);
    return _data;
  }

 private:
  char const* _data;
};

//------------------------------------------------------------------------------

inline void swap(shared_string& lhs, shared_string& rhs) { lhs.swap(rhs); }

inline char const* string_data(shared_string const& s) { return s.data(); }

inline int string_size(shared_string const& s) { return s.size(); }

template <>
struct is_transferable_impl<shared_string> : std::true_type {};

template <>
struct is_shareable_impl<shared_string> : std::true_type {};

//------------------------------------------------------------------------------

template <>
struct null_traits<shared_string> {
  static constexpr bool is_nullable = true;

  struct storage {
    alignas(shared_string) uintptr_t val;
  };

  static_assert(sizeof(storage) == sizeof(shared_string));
  static_assert(alignof(storage) == alignof(shared_string));

  static constexpr bool is_null(storage const& s) noexcept {
    return s.val == 1;
  }

  static void write_null(storage& s) noexcept { s.val = 1; }
};

}  // namespace rethink
