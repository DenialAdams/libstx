// See LICENSE file for copyright and license details
#include "internal.h"

void
ustr_free(Ustring const *sp)
{
	free(sp->mem);
}
