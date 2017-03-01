#include "oystr.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

static int ctest_ret = 0;
#define ctest_assert(x) (ctest_ret = ctest_ret && (x))
#define CTEST_BEGIN (ctest_begin(__func__))
#define CTEST_END (ctest_end())

static int ctest_passed = 0;
static int ctest_total = 0;

struct ctest {
	int ret;
	int passed;
	int total;
};

void
ctest_intro()
{
	printf("Beginning tests.\n");
}

void
ctest_begin(const char *name)
{
	++ctest_total;
	ctest_ret = 1;
	printf("- Testing %s ... ", name);
}

void
ctest_end()
{
	if (ctest_ret)
		++ctest_passed;
	printf("%s\n", ctest_ret ? "pass" : "fail");
}

void
ctest_summary()
{
	printf("Total tests: %d\n", ctest_total);
	printf("Tests passed: %d\n", ctest_passed);
	printf("Tests failed: %d\n", ctest_total - ctest_passed);
}

// ----------------------------------------------------------------------------
// Begin unit test section
// ----------------------------------------------------------------------------

void
test_oystr_init()
{
	struct oystr s1;

	CTEST_BEGIN;
	oystr_init(&s1);
	ctest_assert(NULL == s1.buf);
	ctest_assert(0 == s1.len);
	ctest_assert(0 == s1.size);
	CTEST_END;
}

void
test_oystr_deinit()
{
	struct oystr s1;
	s1.buf = malloc(2);
	s1.buf[0] = 1;
	s1.buf[1] = 2;
	s1.len = 1;
	s1.size = 2;

	CTEST_BEGIN;
	oystr_deinit(&s1);
	ctest_assert(NULL == s1.buf);
	ctest_assert(0 == s1.len);
	ctest_assert(0 == s1.size);
	CTEST_END;
}

void
test_oystr_ensure_size()
{
	int r;
	int i;
	struct oystr s1;
	oystr_init(&s1);

	CTEST_BEGIN;
	for (i=0; i<100; ++i) {
		// 0 as a random value should be ok here.
		r = rand() % 65536;
		int ret;
		ret = oystr_ensure_size(&s1, r);
		ret = ret && ctest_assert(s1.buf);
		ret = ret && ctest_assert(0 == s1.len);
		ret = ret && ctest_assert(r <= s1.size);
		if (ret) {
			// Quit upon first failure
			break;
		}
		oystr_deinit(&s1);
	}
	CTEST_END;
}

void
test_oystr_init_buf()
{
	struct oystr s1;

	CTEST_BEGIN;
	ctest_assert(0 == oystr_init_buf(&s1, 20));
	ctest_assert(NULL != s1.buf);
	ctest_assert(0 == s1.len);
	ctest_assert(20 <= s1.size);
	CTEST_END;

	oystr_deinit(&s1);
}

void
test_oystr_valid()
{
	struct oystr *s1 = NULL;
	struct oystr s2;

	assert(0 == oystr_init_buf(&s2, 10));

	CTEST_BEGIN;
	ctest_assert(0 > oystr_valid(s1));
	ctest_assert(0 == oystr_valid(&s2));
	CTEST_END;

	oystr_deinit(&s2);
}

void
test_oystr_set_len()
{
	int i;
	int ret;
	struct oystr s1;
	assert(0 == oystr_init_buf(&s1, 20));
	memset(s1.buf, 1, 20);

	CTEST_BEGIN;
	for (i=0; i<20; ++i) {
		ctest_assert(0 == oystr_set_len(&s1, i));
		ctest_assert(i == s1.len);
		ctest_assert('\0' == s1.buf[i]);
		ctest_assert(20 <= s1.size);
	}
	CTEST_END;

	oystr_deinit(&s1);
}

void
test_oystr_assign()
{
	int i;
	int ret;
	struct oystr s1;
	oystr_init(&s1);
	CTEST_BEGIN;
	for (i=0; i<11; ++i) {
		ctest_assert(0 == oystr_assign(&s1, "hello world", i));
		ctest_assert(0 == strncmp(s1.buf, "hello world", i));
		ctest_assert(i == s1.len);
		ctest_assert(i <= s1.size);
	}
	CTEST_END;

	oystr_deinit(&s1);
}

void
test_oystr_append()
{
	struct oystr s1;
	oystr_init(&s1);
	assert(0 == oystr_assign(&s1, "hello ", 6));
	CTEST_BEGIN;
	ctest_assert(0 == oystr_append(&s1, "world", 5));
	ctest_assert(0 == strcmp(s1.buf, "hello world"));
	ctest_assert(11 == s1.len);
	ctest_assert(11 <=  s1.size);
	CTEST_END;
	oystr_deinit(&s1);
}

void
test_oystr_write()
{
}

void
test_oystr_insert()
{
}

void
test_oystr_eq()
{
	struct oystr s1;
	struct oystr s2;
	struct oystr s3;
	oystr_init(&s1);
	oystr_init(&s2);
	oystr_init(&s3);
	assert(0 == oystr_assign(&s1, "hello", 5));
	assert(0 == oystr_assign(&s2, "hello", 5));
	assert(0 == oystr_assign(&s3, "wickedpissah", 12));
	CTEST_BEGIN;
	ctest_assert(true == oystr_eq(&s1, &s1));
	ctest_assert(true == oystr_eq(&s1, &s2));
	ctest_assert(false == oystr_eq(&s2, &s3));
	CTEST_END;
	oystr_deinit(&s1);
	oystr_deinit(&s2);
	oystr_deinit(&s3);
}

void
test_oystr_swap()
{
	struct oystr s1;
	struct oystr s2;
	oystr_init(&s1);
	oystr_init(&s2);
	assert(0 == oystr_assign(&s1, "buffer_one", 10));
	assert(0 == oystr_assign(&s2, "buf_2", 5));
	CTEST_BEGIN;
	oystr_swap(&s1, &s2);

	ctest_assert(0 == strcmp(s1.buf, "buf_2"));
	ctest_assert(5 == s1.len);
	ctest_assert(5 <= s1.size);

	ctest_assert(0 == strcmp(s2.buf, "buffer_one"));
	ctest_assert(10 == s2.len);
	ctest_assert(10 <= s2.size);
	CTEST_END;
	oystr_deinit(&s1);
	oystr_deinit(&s2);
}

void
test_oystr_trunc()
{
	const char tstring[] = "truncateme";
	int i;
	struct oystr s1;
	oystr_init(&s1);
	assert(0 == oystr_assign(&s1, tstring, 10));
	CTEST_BEGIN;
	for (i=1; i<=10; ++i) {
		ctest_assert(tstring[10-i] == oystr_trunc(&s1, 1));
		ctest_assert(10 - i == s1.len);
		ctest_assert(11 <= s1.size);
	}
	ctest_assert('\0' == oystr_trunc(&s1, 20));
	CTEST_END;
	oystr_deinit(&s1);
}

int
main()
{
	ctest_intro();

	test_oystr_init();
	test_oystr_deinit();
	test_oystr_ensure_size();
	test_oystr_init_buf();
	test_oystr_valid();
	test_oystr_set_len();

	test_oystr_assign();
	test_oystr_append();
	//TODO test_oystr_write();
	//TODO test_oystr_insert();

	test_oystr_eq();
	test_oystr_swap();
	test_oystr_trunc();

	ctest_summary();
	return !(ctest_passed == ctest_total);
}
