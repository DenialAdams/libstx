#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../libstx.h"
#include "test.h"

// Random strings generated for the test.
char rs1[1024];
char rs2[1024];
char rb1[1024];
char rb2[1024];

TEST_DEFINE(stxcmp_empty)
{
	stx s1 = {0};
	stx s2 = {0};

	TEST_ASSERT(true == stxcmp(stxr(s1), stxr(s2)));

	TEST_END;
}

TEST_DEFINE(stxcmp_diff_str)
{
	stx s1 = {
		.len = strlen(rs1),
		.size = sizeof(rs1),
		.mem = rs1,
	};

	stx s2 = {
		.len = strlen(rs2),
		.size = sizeof(rs2),
		.mem = rs2,
	};

	TEST_ASSERT(false == stxcmp(stxr(s1), stxr(s2)));

	TEST_END;
}

TEST_DEFINE(stxcmp_same_str)
{
	stx s1 = {
		.len = strlen(rs1),
		.size = sizeof(rs1),
		.mem = rs1,
	};

	TEST_ASSERT(true == stxcmp(stxr(s1), stxr(s1)));

	TEST_END;
}

TEST_DEFINE(stxcmp_diff_bytes)
{
	stx s1 = {
		.len = strlen(rb1),
		.size = sizeof(rb1),
		.mem = rb1,
	};

	TEST_ASSERT(true == stxcmp(stxr(s1), stxr(s1)));

	TEST_END;
}

TEST_DEFINE(stxcmp_same_bytes)
{
	stx s1 = {
		.len = sizeof(rb1),
		.size = sizeof(rb1),
		.mem = rb1,
	};

	TEST_ASSERT(true == stxcmp(stxr(s1), stxr(s1)));

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

	// Ensure that they are different.
	while (0 == memcmp(rb1, rb2, sizeof(rb1))) {
		test_rand_bytes(rb2, sizeof(rb2));
	}

	while (0 == strcmp(rs1, rs2)) {
		test_rand_str(rs2, sizeof(rs2));
	}

	TEST_INIT(ts);
	TEST_RUN(ts, stxcmp_empty);
	TEST_RUN(ts, stxcmp_diff_str);
	TEST_RUN(ts, stxcmp_same_str);
	TEST_RUN(ts, stxcmp_diff_bytes);
	TEST_RUN(ts, stxcmp_same_bytes);
	TEST_PRINT(ts);
}
