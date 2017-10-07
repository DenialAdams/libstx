// See LICENSE file for copyright and license details
#include "internal.h"

int
ustr_grow(Ustring *sp, size_t n)
{
	uint8_t *tmp;

	if (internal_size_add_overflows(sp->size, n)) {
		n = SIZE_MAX;
	} else {
		n = sp->size + n;
	}
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
