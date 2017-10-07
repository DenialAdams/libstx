/* See LICENSE file for copyright and license details */
#include "internal.h"

int
ustr_cpy_mem(Ustring *dest, void const *src, size_t n)
{
	if (0 > ustr_ensure_size(dest, ustr_npow2(n + 1)))
		return -1;
	memmove(dest->mem, src, n);
	dest->len = n;
	ustr_term(dest);
	return 0;
}

int
ustr_cpy_cstr(Ustring *dest, char const *src)
{
	return ustr_cpy_mem(dest, src, strlen(src));
}

int
ustr_cpy(Ustring *dest, Ustr const *src)
{
	return ustr_cpy_mem(dest, src->mem, src->len);
}
