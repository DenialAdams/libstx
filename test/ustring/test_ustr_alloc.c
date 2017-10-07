#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../ustring.h"
#include "test.h"

TEST_DEFINE(ustr_alloc_zero)
{
	Ustring * s1;

	TEST_ASSERT(0 == ustr_alloc(&s1, 0));
	TEST_ASSERT(NULL == s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(0 == s1.size);

	ustr_free(&s1);

	TEST_END;
}

TEST_DEFINE(ustr_alloc_16bit_max)
{
	Ustring * s1;

	TEST_ASSERT(0 == ustr_alloc(&s1, 65535));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(65535 == s1.size);

	ustr_free(&s1);

	TEST_END;
}

TEST_DEFINE(ustr_alloc_twice_rand)
{
	Ustring * s1;
	uint8_t *p;
	size_t n1 = test_rand(1, 65535);
	size_t n2 = test_rand(1, 65535);

	TEST_ASSERT(0 == ustr_alloc(&s1, n1));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(n1 == s1.size);

	p = s1.mem;

	TEST_ASSERT(0 == ustr_alloc(&s1, n2));
	TEST_ASSERT(p != s1.mem);
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(n2 == s1.size);

	ustr_free(&s1);
	free(p);

	TEST_END;
}

int
main(void)
{
	srand(time(NULL));
	TEST_INIT(ts);
	TEST_RUN(ts, ustr_alloc_zero);
	TEST_RUN(ts, ustr_alloc_16bit_max);
	TEST_RUN(ts, ustr_alloc_twice_rand);
	TEST_PRINT(ts);
}
