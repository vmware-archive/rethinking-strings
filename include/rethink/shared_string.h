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
  shared_string() = default;

  shared_string(shared_string const& rhs) : _data(rhs._data) {
    detail::retain_ctrl_block(_data);
  }

  shared_string& operator=(shared_string const& rhs) {
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
  template <class T>
  shared_string(T r) : _data(detail::new_ctrl_block(r)){};

  template <class T>
  shared_string& operator=(T r) {
    shared_string tmp(r);
    swap(tmp);
    return *this;
  }

  template <class T, std::enable_if_t<!std::is_same_v<T, shared_string>>>
  shared_string(T&& rhs) {
    if
      constexpr(is_transferable_v<decltype(rhs)>) {
        _data = std::forward<T>(rhs).transfer();
      }
    else {
      _data = detail::new_ctrl_block(rhs);
    }
  }

  template <class T, std::enable_if_t<!std::is_same_v<T, shared_string>>>
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
      return const_cast<char*>(detach());
    }
    return detail::new_ctrl_block(*this);
  }

 private:
  char const* detach() noexcept {
    char const* tmp = _data;
    _data = nullptr;
    return tmp;
  }

 private:
  char const* _data{nullptr};
};

//------------------------------------------------------------------------------

inline void swap(shared_string& lhs, shared_string& rhs) { lhs.swap(rhs); }
inline char const* string_data(shared_string const& s) { return s.data(); }
inline int string_size(shared_string const& s) { return s.size(); }

template <>
struct is_transferable_impl<shared_string> : std::true_type {};

template <>
struct is_shareable_impl<shared_string> : std::true_type {};

}  // namespace rethink
