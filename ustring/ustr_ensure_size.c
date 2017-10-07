/* See LICENSE file for copyright and license details */
#include "internal.h"

int
ustr_ensure_size(Ustring *sp, size_t n)
{
	uint8_t *tmp;

	if (sp->size >= n)
		return 0;
	if (!sp->mem && !n) {
		memset(sp, 0, sizeof(*sp));
	} else {
		tmp = realloc(sp->mem, n);
		if (!tmp)
			return -1;
		sp->mem = tmp;
		sp->size = n;
	}
	return 0;
}
