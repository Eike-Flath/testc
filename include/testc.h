#ifndef TESTC_H_
#define TESTC_H_ 1

#include <stdbool.h>
#include <stddef.h>

#ifndef __has_include
#warning "compiler doesn't support __has_include"
#define __has_include(h) 1
#endif

#ifndef __has_attribute
#define __has_attribute(a) 0
#endif

#if __has_include("stdlib.h")
#include <stdlib.h>
#define TC_EXIT_SUCCESS EXIT_SUCCESS
#define TC_EXIT_FAILURE EXIT_FAILURE
#else
#define TC_EXIT_SUCCESS 0
#define TC_EXIT_FAILURE 1
#endif /* __has_include("stdlib.h") */

#if __has_include("stdio.h")
#include <stdio.h>
#define TC_PRINTF(f, ...) fprintf(stderr, f, __VA_ARGS__)
#else
int testc_printf_impl_(const char *fmt, ...);
#define TC_PRINTF(f, ...) testc_printf_impl_(f, __VA_ARGS__)
#endif /* __has_include("stdio.h") */

#if !defined(TC_ATTR_CONSTRUCTOR) && __has_attribute(constructor)
#define TC_ATTR_CONSTRUCTOR __attribute__((constructor))
#endif

#ifndef TC_ATTR_CONSTRUCTOR
#error "TC_ATTR_CONSTRUCTOR isn't defined"
#endif

struct testc_test_node_ {
  struct testc_test_node_ *next;
  const char *name;
  void (*test)(void);
};

extern struct testc_test_node_ *testc_test_root_node_;
extern bool testc_test_failed_;

#define TC_MAIN                                                                \
  struct testc_test_node_ *testc_test_root_node_ = NULL;                       \
  bool testc_test_failed_;                                                     \
  int main(int argc, char *const *const argv) {                                \
    (void)argc;                                                                \
    (void)argv;                                                                \
    struct testc_test_node_ *n = testc_test_root_node_;                        \
    int exit_code = TC_EXIT_SUCCESS;                                           \
    int passed = 0, total = 0;                                                 \
    while (n) {                                                                \
      TC_PRINTF("Running test '%s'...\n", n->name);                            \
      testc_test_failed_ = false;                                              \
      n->test();                                                               \
      if (testc_test_failed_) {                                                \
        exit_code = TC_EXIT_FAILURE;                                           \
        TC_PRINTF("'%s' FAILED\n", n->name);                                   \
      } else {                                                                 \
        TC_PRINTF("'%s' PASSED\n", n->name);                                   \
        passed++;                                                              \
      }                                                                        \
      total++;                                                                 \
      n = n->next;                                                             \
    }                                                                          \
    TC_PRINTF("Passed %d/%d test(s).\n", passed, total);                       \
    return exit_code;                                                          \
  }

#define TC_TEST(m_n)                                                           \
  static void testc_test_##m_n##_(void);                                       \
  TC_ATTR_CONSTRUCTOR static void register_testc_test_##m_n##_(void) {         \
    static struct testc_test_node_ node = {0};                                 \
    node.next = testc_test_root_node_;                                         \
    node.name = #m_n;                                                          \
    node.test = testc_test_##m_n##_;                                           \
    testc_test_root_node_ = &node;                                             \
  }                                                                            \
  static void testc_test_##m_n##_(void)

#define TC_ASSERT(cond, msg_fmt, ...)                                          \
  do {                                                                         \
    if (!(cond)) {                                                             \
      TC_PRINTF(msg_fmt, __VA_ARGS__);                                         \
      testc_test_failed_ = true;                                               \
    }                                                                          \
  } while (0)

#define TC_ASSERT_TRUE(cond)                                                   \
  TC_ASSERT(cond, "Assertion failed: expected condition `%s` to be true\n",    \
            #cond);
#define TC_ASSERT_FALSE(cond)                                                  \
  TC_ASSERT(!(cond),                                                           \
            "Assertion failed: expected condition `%s` to be false\n", #cond);
#define TC_ASSERT_EQ(a, b)                                                     \
  TC_ASSERT((a) == (b), "Assertion failed: `%s` didn't equal `%s`\n", #a, #b)
#define TC_ASSERT_NE(a, b)                                                     \
  TC_ASSERT((a) != (b), "Assertion failed: `%s` equaled `%s`\n", #a, #b)
#define TC_ASSERT_NULL(a)                                                      \
  TC_ASSERT((a) == NULL, "Assertion failed: `%s` wasn't null\n", #a)
#define TC_ASSERT_NONNULL(a)                                                   \
  TC_ASSERT((a) != NULL, "Assertion failed: `%s` was null\n", #a)
#define TC_ASSERT_GE(a, b)                                                     \
  TC_ASSERT((a) >= (b),                                                        \
            "Assertion failed: `%s` wasn't greater or equal than `%s`\n", #a,  \
            #b)
#define TC_ASSERT_LE(a, b)                                                     \
  TC_ASSERT((a) <= (b),                                                        \
            "Assertion failed: `%s` wasn't less or equal than `%s`\n", #a, #b)

#endif /* TESTC_H_ */
