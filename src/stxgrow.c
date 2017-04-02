// See LICENSE file for copyright and license details
#include "internal.h"

int
stxgrow(stx *s1, size_t n)
{
	char *tmp;

	if (internal_size_add_overflows(s1->size, n))
		n = SIZE_MAX;

	tmp = realloc(s1->mem, s1->size + n);
	if (!tmp)
		return -1;

	s1->mem = tmp;
	s1->size += n;
	return 0;
}
