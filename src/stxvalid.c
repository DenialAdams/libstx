// See LICENSE file for copyright and license details
#include "internal.h"

bool
stxvalid(stx *sp)
{
	if (!sp)
		return false;
	if (!sp->mem || sp->size < sp->len)
		return false;

	return true;
}
