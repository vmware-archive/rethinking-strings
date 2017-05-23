// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/api.h>

#include <string>
#include <type_traits>

namespace rethink {

//------------------------------------------------------------------------------

static_assert(sizeof(void*) == 8);

//------------------------------------------------------------------------------

template <class T, class = std::enable_if_t<std::is_pointer_v<T>>>
int string_size(T t) {
  // TODO runtime assert if > INT_MAX.
  return std::strlen(t);
}

template <int N>
constexpr int string_size(char const (&)[N]) {
  return N - 1;
}

//------------------------------------------------------------------------------

template <class T, class = std::enable_if_t<std::is_pointer_v<T>>>
constexpr const char* string_data(T t) {
  return t;
}

template <int N>
constexpr const char* string_data(char const (&a)[N]) {
  return &a[0];
}

//------------------------------------------------------------------------------

template <class T>
struct is_transferable_impl : std::false_type {};

template <class T>
struct is_transferable : std::conjunction<is_transferable_impl<std::decay_t<T>>,
                                          std::is_rvalue_reference<T>> {};

template <class T>
inline constexpr bool is_transferable_v = is_transferable<T>::value;

//------------------------------------------------------------------------------

template <class T>
struct is_shareable_impl : std::false_type {};

template <class T>
struct is_shareable : is_shareable_impl<std::decay_t<T>> {};

template <class T>
inline constexpr bool is_shareable_v = is_shareable<T>::value;

}  // namespace rethink
