// See LICENSE file for copyright and license details
#include "internal.h"

bool
stxcmp(const spx s1, const spx s2)
{
	size_t i;

	// First, check if they're the same length.
	if (s1.len != s2.len)
		return false;

	// Now compare chunks of register size.
	const size_t chunks = s1.len / sizeof(i);
	for (i = 0; i < chunks; ++i) {
		if (*(size_t *)s1.mem + i != *(size_t *)s2.mem + i)
			return false;
	}

	// Compare the individual leftover bytes.
	const size_t n = chunks * sizeof(i);
	for (i = 0; i < s1.len % sizeof(i); ++i) {
		if (s1.mem[n + i] != s2.mem[n + i])
			return false;
	}

	return true;
}
