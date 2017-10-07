/* See LICENSE file for copyright and license details */
#include "internal.h"

int
ustr_dup_mem(Ustring *dest, void const *src, size_t n)
{
	if (0 < ustr_alloc(dest, n))
		return -1;
	memmove(dest->mem, src, n);
	dest->len = n;
	ustr_term(dest);
	return 0;
}

int
ustr_dup_cstr(Ustring *dest, char const *src)
{
	return ustr_dup_mem(dest, src, strlen(src));
}

int
ustr_dup(Ustring *dest, Ustr const *src)
{
	return ustr_dup_mem(dest, src->mem, src->len);
}
