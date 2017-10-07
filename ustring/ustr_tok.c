// See LICENSE file for copyright and license details
#include "internal.h"

bool
ustr_tok_mem(Ustr *dest, Ustr *pos, void const *delim, size_t n)
{
	size_t i, j;

	for (i=0; i<pos->len; ++i) {
		for (j=i; j<i+n; ++j) {
			if (pos->mem[j] != ((uint8_t *)delim)[j-i]) {
				break;
			}
		}
		if (j-i == n) {
			// Create the token.
			dest->mem = pos->mem;
			dest->len = i;
			// Move the reference forward.
			pos->mem += i + n;
			pos->len -= i + n;
			return true;
		}
	}
	return false;
}

bool
ustr_tok_cstr(Ustr *dest, Ustr *pos, char const *delim)
{
	return ustr_tok_mem(dest, pos, delim, strlen(delim));
}

bool
ustr_tok(Ustr *dest, Ustr *pos, Ustr const *delim)
{
	return ustr_tok_mem(dest, pos, delim->mem, delim->len);
}
