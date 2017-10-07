// See LICENSE file for copyright and license details
#include "internal.h"

Ustr *
ustr_trunc(Ustr *sp, size_t n)
{
	if (n >= sp->len) {
		sp->len = 0;
	} else {
		sp->len -= n;
	}
	return sp;
}
