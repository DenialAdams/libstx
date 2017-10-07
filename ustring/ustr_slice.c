// See LICENSE file for copyright and license details
#include "internal.h"

void
ustr_slice(Ustr *dest, Ustr const *src, size_t begin, size_t end)
{
	dest->mem = src->mem + begin;
	dest->len = end - begin;
}
