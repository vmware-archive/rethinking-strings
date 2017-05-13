#include <rethink/rethink.h>

#include <stdio.h>
#include <string>

using namespace std;
using namespace rethink;

constexpr ref_string kShort{"foo"};
static_assert(kShort.size() == 4);
constexpr ref_string kLong{"supercalifragilisticexpialidocious"};

void print(ref_string s) {
  printf("%.*s", static_cast<int>(s.size()), s.data());
}

struct foo {
  foo(ref_string s) : _s(s) {}
  void output() { print(_s); }
  shared_string _s;
};
int main() {
  foo short_foo{kShort};
  short_foo.output();
  foo long_foo{kLong};
  long_foo.output();
  return 0;
}
