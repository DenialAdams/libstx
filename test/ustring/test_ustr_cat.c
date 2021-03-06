#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include "test.h"
#include "ustring.h"

// Random strings generated for the test.
uint8_t rs1[1024];
uint8_t rs2[1024];
uint8_t rb1[1024];
uint8_t rb2[1024];

TEST_DEFINE(ustr_cat_mem_zero)
{
	Ustring s1 = {0};

	TEST_ASSERT(0 == ustr_cat_mem(&s1, NULL, 0));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT('\0' == s1.mem[s1.len]);
	TEST_ASSERT(0 == s1.len);
	TEST_ASSERT(1 <= s1.size);

	ustr_free(&s1);

	TEST_END;
}

TEST_DEFINE(ustr_cat_mem_once)
{
	Ustring s1 = {0};

	TEST_ASSERT(0 == ustr_cat_mem(&s1, rb1, sizeof(rb1)));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == memcmp(s1.mem, rb1, sizeof(rb1)));
	TEST_ASSERT('\0' == s1.mem[s1.len]);
	TEST_ASSERT(sizeof(rb1) == s1.len);
	TEST_ASSERT(sizeof(rb1) + 1 <= s1.size);

	ustr_free(&s1);

	TEST_END;
}

TEST_DEFINE(ustr_cat_mem_twice)
{
	Ustring s1 = {0};
	char tstr[sizeof(rb1) + sizeof(rb2)];

	memcpy(tstr, rb1, sizeof(rb1));
	memcpy(tstr + sizeof(rb1), rb2, sizeof(rb2));
	const size_t tlen = sizeof(rb1) + sizeof(rb2);

	TEST_ASSERT(0 == ustr_cat_mem(&s1, rb1, sizeof(rb1)));
	TEST_ASSERT(0 == memcmp(s1.mem, rb1, sizeof(rb1)));
	TEST_ASSERT('\0' == s1.mem[s1.len]);
	TEST_ASSERT(sizeof(rb1) == s1.len);
	TEST_ASSERT(sizeof(rb1) + 1 <= s1.size);

	TEST_ASSERT(0 == ustr_cat_mem(&s1, rb2, sizeof(rb2)));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == memcmp(s1.mem, tstr, tlen));
	TEST_ASSERT('\0' == s1.mem[s1.len]);
	TEST_ASSERT(tlen == s1.len);
	TEST_ASSERT(tlen + 1 <= s1.size);

	ustr_free(&s1);

	TEST_END;
}

TEST_DEFINE(ustr_cat_mem_bytes_seperately)
{
	Ustring s1 = {0};
	size_t i;

	for (i = 0; i < sizeof(rb1); ++i) {
		TEST_ASSERT(0 == ustr_cat_byte(&s1, rb1[i]));
		TEST_ASSERT(NULL != s1.mem);
		TEST_ASSERT(s1.mem[i] == rb1[i]);
		TEST_ASSERT('\0' == s1.mem[s1.len]);
		TEST_ASSERT(i + 1 == s1.len);
		TEST_ASSERT(i + 1 <= s1.size);
	}
	TEST_ASSERT(sizeof(rb1) + 1 <= s1.size);
	TEST_ASSERT(0 == memcmp(s1.mem, rb1, i));

	ustr_free(&s1);

	TEST_END;
}

TEST_DEFINE(ustr_cat_cstr_once)
{
	Ustring s1 = {0};

	TEST_ASSERT(0 == ustr_cat_cstr(&s1, rs1));
	TEST_ASSERT(NULL != s1.mem);
	TEST_ASSERT(0 == memcmp(s1.mem, rs1, strlen(rs1)));
	TEST_ASSERT(strlen(rs1) == s1.len);
	TEST_ASSERT(strlen(rs1) + 1 <= s1.size);

	ustr_free(&s1);

	TEST_END;
}

TEST_DEFINE(ustr_cat_cstr_twice)
{
	Ustring s1;
	char tstr[sizeof(rs1) + sizeof(rs2)];

	strcpy(tstr, rs1);
	strcpy(tstr + strlen(rs1), rs2);
	const size_t tlen = strlen(tstr);
	ustr_alloc(&s1, strlen(rs1) + strlen(rs2));

	TEST_ASSERT(0 == ustr_cat_cstr(&s1, rs1));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, rs1, sizeof(rs1)));
	TEST_ASSERT('\0' == s1.mem[s1.len]);
	TEST_ASSERT(strlen(rs1) == s1.len);
	TEST_ASSERT(sizeof(rs1) + 1 <= s1.size);

	TEST_ASSERT(0 == ustr_cat_cstr(&s1, rs2));
	TEST_ASSERT(s1.mem != NULL);
	TEST_ASSERT(0 == memcmp(s1.mem, tstr, tlen));
	TEST_ASSERT('\0' == s1.mem[s1.len]);
	TEST_ASSERT(tlen == s1.len);
	TEST_ASSERT(tlen + 1 <= s1.size);

	ustr_free(&s1);

	TEST_END;
}

int
main(void)
{
	srand(time(NULL));
	test_rand_bytes(rb1, sizeof(rb1));
	test_rand_bytes(rb2, sizeof(rb2));
	test_rand_str(rs1, sizeof(rs1));
	test_rand_str(rs2, sizeof(rs2));

	TEST_INIT(ts);
	TEST_RUN(ts, ustr_cat_mem_zero);
	TEST_RUN(ts, ustr_cat_mem_once);
	TEST_RUN(ts, ustr_cat_mem_twice);
	TEST_RUN(ts, ustr_cat_mem_bytes_seperately);
	TEST_RUN(ts, ustr_cat_cstr_once);
	TEST_RUN(ts, ustr_cat_cstr_twice);
	TEST_PRINT(ts);
}
