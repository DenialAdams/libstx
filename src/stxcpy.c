// See LICENSE file for copyright and license details
#include "internal.h"

stx *
stxcpy_mem(stx *sp, const char *src, size_t len)
{
	memcpy(sp->mem, src, len);
	sp->len = len;

	return sp;
}

stx *
stxcpy_str(stx *sp, const char *src)
{
	sp->len += internal_strncpy(sp->mem, src, sp->size);
	return sp;
}

stx *
stxcpy_stx(stx *dst, const stx *src)
{
	dst = stxnew(dst, src->len);
	return stxcpy_mem(dst, src->mem, src->len);
}
