// See LICENSE file for copyright and license details
#include "internal.h"

Ustring *
ustr_lstrip_mem(Ustring *sp, void const *chs, size_t len)
{
	size_t removed = 0;
	uint8_t *begin = sp->mem;
	uint8_t *end = sp->mem + sp->len;

	while (begin != end && memchr(chs, *begin, len)) {
		++removed;
		++begin;
	}
	if (begin != sp->mem)
		memmove(sp->mem, begin, sp->len - removed);
	sp->len -= removed;
	return sp;
}

Ustring *
ustr_rstrip_mem(Ustring *sp, void const *chs, size_t len)
{
	size_t removed = 0;
	uint8_t *begin = sp->mem + sp->len - 1;
	uint8_t *end = sp->mem;

	if (0 == sp->len)
		return sp;
	while (begin != end && memchr(chs, *begin, len)) {
		++removed;
		--begin;
	}
	sp->len -= removed;
	return sp;
}

Ustring *
ustr_strip_mem(Ustring *sp, void const *chs, size_t len)
{
	sp = ustr_lstrip_mem(sp, chs, len);
	sp = ustr_rstrip_mem(sp, chs, len);
	return sp;
}

Ustring *
ustr_lstrip_cstr(Ustring *sp, char const *chs)
{
	return ustr_lstrip_mem(sp, chs, strlen(chs));
}

Ustring *
ustr_rstrip_cstr(Ustring *sp, char const *chs)
{
	return ustr_rstrip_mem(sp, chs, strlen(chs));
}

Ustring *
ustr_strip_cstr(Ustring *sp, char const *chs)
{
	return ustr_strip_mem(sp, chs, strlen(chs));
}

Ustring *
ustr_lstrip(Ustring *sp, Ustr const *chs)
{
	return ustr_lstrip_mem(sp, chs->mem, chs->len);
}

Ustring *
ustr_rstrip(Ustring *sp, Ustr const *chs)
{
	return ustr_rstrip_mem(sp, chs->mem, chs->len);
}

Ustring *
ustr_strip(Ustring *sp, Ustr const *chs)
{
	return ustr_strip_mem(sp, chs->mem, chs->len);
}
