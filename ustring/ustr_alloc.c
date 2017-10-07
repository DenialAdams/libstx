/* See LICENSE file for copyright and license details */
#include "internal.h"

int
ustr_alloc(Ustring *sp, size_t n)
{
	sp->len = 0;
	if (0 == n) {
		sp->mem = NULL;
		sp->size = 0;
		return 0;
	}
	sp->mem = malloc(n);
	if (!sp->mem) {
		sp->mem = NULL;
		return -1;
	}
	sp->size = n;
	return 0;
}
