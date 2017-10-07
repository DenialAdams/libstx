/* See LICENSE file for copyright and license details */
#include <stdlib.h>
#include <stdint.h>

size_t
ustr_npow2(size_t n)
{
	size_t next = 1;

	if (n >= SIZE_MAX / 2) {
		next = SIZE_MAX;
	} else {
		while (next < n)
			next <<= 1;
	}
	return next;
}
