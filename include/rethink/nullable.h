// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#pragma once

#include <rethink/rethink.h>

#include <type_traits>
#include <utility>

namespace rethink {

//------------------------------------------------------------------------------

template <class T, class Enable = void>
class nullable;

//------------------------------------------------------------------------------
// nullable<T> Default partial specialization that holds a T and a bool.

template <class T>
class nullable<T, std::enable_if_t<!is_nullable_v<T>>> {
  template <class R>
  static constexpr inline bool is_this_type_v =
      std::is_same_v<nullable, std::decay_t<R>>;

 public:
  nullable() : _is_set(false) {}

  ~nullable() {
    if (is_set()) {
      destroy();
    }
  }

  template <class R>
  nullable(R&& rhs) {
    if
      constexpr(is_this_type_v<R>) {
        if (rhs.is_set()) {
          construct(std::forward<R>(rhs).get());
        } else {
          _is_set = false;
        }
      }
    else {
      construct(std::forward<R>(rhs));
    }
  }

  template <class R>
  nullable& operator=(R&& rhs) {
    set(std::forward<R>(rhs));
    return *this;
  }

  template <class R>
  void set(R&& rhs) {
    if
      constexpr(is_this_type_v<R>) {
        if (this == &rhs) {
          return;
        }
      }
    nullable tmp(std::forward<R>(rhs));
    swap(*this, tmp);
  }

  friend void swap(nullable& lhs, nullable& rhs) {
    if (lhs.is_set()) {
      if (rhs.is_set()) {
        using std::swap;
        swap(lhs.get(), rhs.get());
      } else {
        rhs.construct(std::move(lhs.get()));
        lhs.unset();
      }
    } else {
      if (rhs.is_set()) {
        lhs.construct(std::move(rhs.get()));
        rhs.unset();
      }
    }
  }

  void unset() {
    if (is_set()) {
      destroy();
      _is_set = false;
    }
  }

  bool is_set() const { return _is_set; }

  T& get() & { return _val; }

  T const& get() const & { return _val; }

  T&& get() && { return std::move(_val); }

 private:
  template <class U>
  void construct(U&& u) {
    new (&_val) T(std::forward<U>(u));
    _is_set = true;
  }

  void destroy() { _val.~T(); }

 private:
  union {
    char _unused;
    T _val;
  };
  bool _is_set;
};

//------------------------------------------------------------------------------
// nullable<T> Bit-stealing partial specialization that has sizeof(T).

template <class T>
class nullable<T, std::enable_if_t<is_nullable_v<T>>> {
  using traits = null_traits<T>;

  template <class R>
  static constexpr inline bool is_this_type_v =
      std::is_same_v<nullable, std::decay_t<R>>;

 public:
  nullable() { traits::write_null(_storage); }

  ~nullable() {
    if (is_set()) {
      destroy();
    }
  }

  template <class R>
  nullable(R&& rhs) {
    if
      constexpr(is_this_type_v<R>) {
        if (rhs.is_set()) {
          construct(std::forward<R>(rhs).get());
        } else {
          traits::write_null(_storage);
        }
      }
    else {
      construct(std::forward<R>(rhs));
    }
  }

  template <class R>
  nullable& operator=(R&& rhs) {
    set(std::forward<R>(rhs));
    return *this;
  }

  template <class R>
  void set(R&& rhs) {
    if
      constexpr(is_this_type_v<R>) {
        if (this == &rhs) {
          return;
        }
      }
    nullable tmp(std::forward<R>(rhs));
    swap(*this, tmp);
  }

  friend void swap(nullable& lhs, nullable& rhs) {
    if (lhs.is_set()) {
      if (rhs.is_set()) {
        using std::swap;
        swap(lhs.get(), rhs.get());
      } else {
        rhs.construct(std::move(lhs.get()));
        lhs.unset();
      }
    } else {
      if (rhs.is_set()) {
        lhs.construct(std::move(rhs.get()));
        rhs.unset();
      }
    }
  }

  void unset() {
    if (is_set()) {
      destroy();
      traits::write_null(_storage);
    }
  }

  bool is_set() const { return !traits::is_null(_storage); }

  T& get() & { return _val; }

  T const& get() const & { return _val; }

  T&& get() && { return std::move(_val); }

 private:
  template <class U>
  void construct(U&& u) {
    new (&_val) T(std::forward<U>(u));
  }

  void destroy() { _val.~T(); }

 private:
  union {
    typename traits::storage _storage;
    T _val;
  };
};

}  // namespace rethink
