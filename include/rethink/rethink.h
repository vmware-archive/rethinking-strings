// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/api.h>

#include <string>

namespace rethink {

//------------------------------------------------------------------------------

static_assert(sizeof(void*) == 8);

//------------------------------------------------------------------------------

template <int N>
constexpr const char* string_data(char const (&a)[N]) {
  return &a[0];
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

template <class T, class = std::enable_if_t<std::is_pointer_v<T>>>
int string_size(T t) {
  // TODO runtime assert if > INT_MAX.
  return std::strlen(t);
}

}  // namespace rethink
