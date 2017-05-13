// Copyright (c) 2017 VMware, Inc. All Rights Reserved.

#include <rethink/ref_string.h>
#include <rethink/shared_string.h>

#include <stdio.h>
#include <string>

using namespace std;

constexpr string_view kShort{"foo"};
constexpr string_view kLong{"supercalifragilisticexpialidocious"};

void print(string_view s) {
  printf("%.*s", static_cast<int>(s.size()), s.data());
}

struct foo {
  foo(string_view s) : _s(s) {}
  void output() { print(_s); }
  string _s;
};

int main() {
  foo short_foo{kShort};
  short_foo.output();
  foo long_foo{kLong};
  long_foo.output();
  return 0;
}
