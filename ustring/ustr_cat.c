/* See LICENSE file for copyright and license details */
#include "internal.h"

int
ustr_cat_byte(Ustring *dest, uint8_t src)
{
	if (0 > ustr_ensure_size(dest, ustr_npow2(dest->len + 1 + 1)))
		return -1;
	dest->mem[dest->len] = src;
	dest->len += 1;
	ustr_term(dest);
	return 0;
}

int
ustr_cat_mem(Ustring *dest, void const *src, size_t n)
{
	if (0 > ustr_ensure_size(dest, ustr_npow2(dest->len + n + 1)))
		return -1;
	memmove(dest->mem + dest->len, src, n);
	dest->len += n;
	ustr_term(dest);
	return 0;
}

int
ustr_cat_cstr(Ustring *dest, char const *src)
{
	return ustr_cat_mem(dest, src, strlen(src));
}

int
ustr_cat_utf8f32(Ustring *dest, uint32_t src)
{
	uint8_t uni8[4];
	size_t n = ustr_utf8f32(uni8, src);

	if (0 >= n)
		return -1;
	return ustr_cat_mem(dest, uni8, n);
}

int
ustr_cat(Ustring *dest, Ustr const *src)
{
	return ustr_cat_mem(dest, src->mem, src->len);
}
