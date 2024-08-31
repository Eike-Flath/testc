# TestC
A simple header-only C library for unit testing.

## License
Licensed under [GPLv3](./LICENSE).

## Usage
```c
#line 1 "sample.c"
#include "testc.h"

static bool foo(int n) {
  return n == 42;
}

TC_TEST(example_test) {
  TC_ASSERT_EQ(1 + 1, 2);
  TC_ASSERT_TRUE(foo(42));
  TC_ASSERT(!foo(3), "%s", "foo(3) should return false");
}

TC_TEST(failing_test) {
  TC_ASSERT_FALSE(true);
}

// This declares main(int, char**)
TC_MAIN
```

Compile `cc -I$TESTC_DIR/include sample.c`.
Then `./a.out` should output:
```
Running test 'failing_test'...
Assertion failed: expected condition `true` to be false
'failing_test' FAILED
Running test 'example_test'...
'example_test' PASSED
Passed 1/2 test(s).
```
