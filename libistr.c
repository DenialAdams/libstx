/* See LICENSE file for copyright and license details */
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "libistr.h"

/* 
safe_add:
	Safely adds together two size_t values while flagging an error
	if integer overflow would happen.

return -> size_t:
	success: Sum of the two arguments
	range error: SIZE_MAX and errno = ERANGE
*/
static inline size_t safe_add(size_t a, size_t b)
{
	if (a > SIZE_MAX - b) {
		errno = ERANGE;
		return SIZE_MAX;
	} else {
		return a + b;
	}
}

/*
smax:
	simple inline function that returns the largest input. Used instead of a 
	macro for type safety

return -> size_t:
	success: largest value between a and b
*/
static inline size_t smax(size_t a, size_t b)
{
	return (a > b) ? a : b;
}

/* 
smin:
	simple inline function that returns the smallest input. Used instead of a 
	macro for type safety

return -> size_t:
	success: smallest value between a and b
*/
static inline size_t smin(size_t a, size_t b)
{
	return (a < b) ? a : b;
}

/* 
nearest_pow:
	Find the nearest power of two that can contain the given value.
return -> size_t:
	success: The closest power of two larger than num
	range error: SIZE_MAX and errno = ERANGE
 */
static inline size_t nearest_pow(size_t base, size_t num)
{
	// Catch powers of 0, as they will break the loop below
	if (base == 0) return 0;

	// Check if the next pow will overflow
	if (num > SIZE_MAX / 2) {
		errno = ERANGE;
		return SIZE_MAX;
	}
		
	while (base < num) base <<= 1 ;
	return base;
}

/* 
istr_ensure_size:
	A Helper function that guarentees to the caller 
	that, if memory can be allocated successfully, the given istring's
	char buffer will be able to hold at least the amount of requested bytes
return -> istring*):
	success: The original string object
	bad args: NULL and errno = EINVAL
	memory error: NULL and errno = ENOMEM
 */
static istring* istr_ensure_size(istring *string, size_t len)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	// The +1 is not for a '\0', but to allow linking together of malloc'd memory
	if (string->size < len) {
		size_t next_size = nearest_pow(2, len);
		// Use realloc to try to prevent defragmentation if sizes are close
		if (string->size*7 < string->len*8) {
			string->buf = realloc(string->buf, sizeof(*(string->buf)) * (next_size));
			if (NULL == string->buf) {
				free(string);
				errno = ENOMEM;
				return NULL;
			}
		} else {
			// Manually realloc, only copying the useful bytes in the buffer
			char *garbage = string->buf;
			string->buf = malloc(sizeof(*(string->buf)) * (next_size));
			if (NULL == string->buf) {
				free(garbage);
				free(string);
				errno = ENOMEM;
				return NULL;
			}
			memcpy(string->buf, garbage, string->len);
			free(garbage);
		}
		string->size = next_size;
	}

	return string;
}

/* 
istr_init:
	A Helper function that allocates memory for an istring
	and initializes all of it's fields.
return -> istring*:
	success: The pointer to a newly allocated istring
	failure: NULL and errno = ENOMEM
 */
static istring* istr_init(size_t init_size)
{
	istring *string = malloc(sizeof(*string));
	if (NULL == string) {
		errno = ENOMEM;
		return NULL;
	}

	string->buf = NULL;
	string->size = 0;
	string->len = 0;

	if (NULL == istr_ensure_size(string, init_size)) {
		errno = ENOMEM;
		free(string);
		return NULL;
	}

	return string;
}

// PUBLIC LIBRARY FUNCTIONS BELOW

istring* istr_new(const istring *src) 
{
	if (NULL == src) return istr_init(0);

	istring *string = istr_init(src->len);

	if (NULL == string) {
		return NULL;
	}

	return istr_assign_bytes(string, src->buf, src->len);
}

istring* istr_new_bytes(const char *str, size_t bytes_len) 
{
	if (NULL == str) return istr_init(0);

	istring *string = istr_init(bytes_len);

	if (NULL == string) {
		return NULL;
	}

	return istr_assign_bytes(string, str, bytes_len);
}

istring* istr_new_cstr(const char *cstr) 
{
	if (NULL == cstr) return istr_init(0);

	size_t bytes_len = strlen(cstr) + 1;

	istring *string = istr_init(bytes_len);

	if (NULL == string) {
		return NULL;
	}

	return istr_assign_bytes(string, cstr, bytes_len);
}

char* istr_free(istring *string, bool free_buf)
{
	if (string->buf) {
		if (!free_buf) {
			char *tmp = string->buf;
			free(string);
			return tmp;
		}
		free(string->buf);
	}

	if (string) free(string);

	return NULL;
}

char* istr_str(const istring *string)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	return string->buf;
}

size_t istr_len(const istring *string)
{
	if (NULL == string) {
		errno = EINVAL;
		return 0;
	}
	return string->len;
}

size_t istr_size(const istring *string)
{
	if (NULL == string) {
		errno = EINVAL;
		return 0;
	}

	return string->size;
}

int istr_eq(const istring *s1, const istring *s2)
{
	if (NULL == s1 || NULL == s2) {
		errno = EINVAL;
		return -1;
	}
	
	if (s1->len != s2->len) return 1;

	for (size_t i=0; i<s1->len; i++) {
		if (s1->buf[i] != s2->buf[i]) return 1;
	}

	return 0;
}

istring* istr_slice(istring *dest, const istring *src, size_t begin, size_t end)
{
	if (NULL == dest || NULL == src || begin > end) {
		errno = EINVAL;
		return NULL;
	}

	return istr_assign_bytes(dest, src->buf + begin, end - begin);
}

istring* istr_assign_bytes(istring *string, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		errno = EINVAL;
		return NULL;
	}

	if (NULL == istr_ensure_size(string, bytes_len)) {
		errno = ENOMEM;
		return NULL;
	}

	// Don't bother memsetting the buffer, just shorten the logical length
	memcpy(string->buf, bytes, bytes_len);
	string->len = bytes_len;

	return string;
}

istring* istr_assign_cstr(istring *string, const char *cstr)
{
	if (NULL == cstr) {
		errno = EINVAL;
		return NULL;
	}

	return istr_assign_bytes(string, cstr, strlen(cstr)+1);
}

istring* istr_truncate_bytes(istring *string, size_t len)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	string->len = smin(string->len, len);
	return string;
}

/*
//TODO truncates len utf8 characters
istring* istr_truncate_utf8(istring *string, size_t count)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	string->len = smin(string->len, count);
	return string;
}
*/
 
char istr_pop_byte(istring *string)
{
	if (NULL == string) {
		errno = EINVAL;
		return -1;
	}

	string->len -= 1;
	return string->buf[string->len + 1];
}

/*
//TODO pops the next utf8 character sequence
char* istr_pop_utf8(istring *string)
{
	if (NULL == string) {
		errno = EINVAL;
		return NULL;
	}

	return string->buf;
}
*/

istring* istr_write(istring *string, size_t index, const istring *ext)
{
	return istr_write_bytes(string, index, ext->buf, ext->len);
}

istring* istr_write_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		errno = EINVAL;
		return NULL;
	}

	size_t potential_len = safe_add(index, bytes_len);

	if (string->len > potential_len) {
		potential_len = string->len;
	}

	if (NULL == istr_ensure_size(string, potential_len)) {
		errno = ENOMEM;
		return NULL;
	}

	memcpy(string->buf + index, bytes, bytes_len);
	string->len = potential_len;

	return string;
}

/*
//TODO opposite of insert
istring* istr_remove_bytes(istring *string, size_t index, size_t len)
{
	return string;
}
*/

istring* istr_prepend(istring *dest, const istring *src)
{
	if (NULL == dest || NULL == src) {
		errno = EINVAL;
		return NULL;
	}
	
	return istr_prepend_bytes(dest, src->buf, src->len);
}

istring* istr_prepend_bytes(istring *string, const char *bytes, size_t bytes_len)
{
	return istr_insert_bytes(string, 0, bytes, bytes_len);
}

istring* istr_append(istring *dest, const istring *src)
{
	if (NULL == src) {
		errno = EINVAL;
		return NULL;
	}

	return istr_append_bytes(dest, src->buf, src->len);
}

/*
//TODO
istring* istr_append_utf8(istring *string, const char *utf8_str, size_t count)
{
	return string;
}
*/

istring* istr_append_bytes(istring *string, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		errno = EINVAL;
		return NULL;
	}

	return istr_insert_bytes(string, string->len, bytes, bytes_len);
}

istring* istr_insert(istring *dest, size_t index, const istring *src)
{
	if (NULL == dest || NULL == src) {
		errno = EINVAL;
		return NULL;
	}

	return istr_insert_bytes(dest, index, src->buf, src->len);
}

/*
//TODO
istring* istr_insert_utf8(istring *string, size_t index, const char *utf8_str, size_t count)
{
	if (NULL == string || NULL == utf8_str) {
		errno = EINVAL;
		return NULL;
	}
}
*/

istring* istr_insert_bytes(istring *string, size_t index, const char *bytes, size_t bytes_len)
{
	if (NULL == string || NULL == bytes) {
		errno = EINVAL;
		return NULL;
	}

	// Overflow check
	size_t total_len = safe_add(string->len, bytes_len);

	if (NULL == istr_ensure_size(string, total_len)) {
		errno = ENOMEM;
		return NULL;
	}

	if (index < string->len) {
		// Create some space for the str to be inserted
		memmove(string->buf + index + bytes_len, \
		        string->buf + index, \
		        string->len - index);
	} else if (index > string->len) {
		// Initialize the space between original data and newly inserted string
		memset(string->buf + string->len, '\0', index - string->len);
	}

	memcpy(string->buf + index, bytes, bytes_len);
	string->len = total_len;

	return string;
}