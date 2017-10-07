// See LICENSE file for copyright and license details
#include "internal.h"

bool
ustr_find_mem(Ustr *dest, Ustr const *haystack, void const *needle, size_t n)
{
	size_t i, j;

	if (0 == n)
		return false;
	if (haystack->len < n)
		return false;
	for (i = 0; i < haystack->len; ++i) {
		for (j = i; j <i + n; ++j) {
			if (haystack->mem[j] != ((const char *)needle)[j-i]) {
				break;
			}
		}
		if (j - i == n) {
			ustr_slice(dest, haystack, i, j);
			return true;
		}
	}
	return false;
}

bool
ustr_find_cstr(Ustr *dest, Ustr const *haystack, const char *needle)
{
	return ustr_find_mem(dest, haystack, needle, strlen(needle));
}

bool
ustr_find(Ustr *dest, Ustr const *haystack, Ustr const *needle)
{
	return ustr_find_mem(dest, haystack, needle->mem, needle->len);
}
