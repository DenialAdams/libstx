/* See LICENSE file for copyright and license details */
#include "internal.h"

int
ustr_ensure_size(Ustring *sp, size_t n)
{
	uint8_t *tmp;

	if (sp->size >= n)
		return 0;

	tmp = realloc(sp->mem, n);
	if (!tmp)
		return -1;
	sp->mem = tmp;
	sp->size = n;

	return 0;
}
