// See LICENSE file for copyright and license details
#include "internal.h"

int
ustr_ins_mem(Ustring *dest, size_t pos, void const *src, size_t n)
{
	if (0 > ustr_ensure_size(dest, ustr_npow2(dest->len + n + 1)))
		return -1;
	/* Create some space if inserting before the end of the buffer. */
	if (pos < dest->len)
		memmove(dest->mem + pos + n, dest->mem + pos, dest->len - pos);
	memmove(dest->mem + pos, src, n);
	dest->len += n;
	ustr_term(dest);
	return 0; 
}

int
ustr_ins_cstr(Ustring *dest, size_t pos, char const *src)
{
	return ustr_ins_mem(dest, pos, src, strlen(src));
}

int
ustr_ins_utf8f32(Ustring *dest, size_t pos, uint32_t src)
{
	char uni8[4];
	size_t n = ustr_utf8f32(uni8, src);

	if (0 >= n)
		return -1;
	return ustr_ins_mem(dest, pos, uni8, n);
}

int
ustr_ins(Ustring *dest, size_t pos, Ustr const *src)
{
	return ustr_ins_mem(dest, pos, src->mem, src->len);
}
