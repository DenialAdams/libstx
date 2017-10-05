/* See LICENSE file for copyright and license details */
#include "internal.h"

bool
ustr_eq(Ustr const *s1, Ustr const *s2)
{
	size_t i;

	/* Must be the same length */
	if (s1->len != s2->len)
		return false;
	/* Compare chunks of register size for speed */
	const size_t chunks = s1->len / sizeof(i);
	for (i = 0; i < chunks; ++i) {
		if (*((size_t *)s1->mem + i) != *((size_t *)s2->mem + i))
			return false;
	}
	/* Compare the leftover bytes */
	const size_t n = chunks * sizeof(i);
	for (i = 0; i < s1->len % sizeof(i); ++i) {
		if (s1->mem[n + i] != s2->mem[n + i])
			return false;
	}
	return true;
}
