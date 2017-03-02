/* See LICENSE file for copyright and license details */
#ifndef OYSTR_H
#define OYSTR_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define OYSTR_OK 0
#define OYSTR_ERR -1

/**
 * The oystr structure for holding multibyte data.
 *
 * All of the fields in this struct are public and available for
 * direct modification and assignment, just don't expect the oystr functions
 * to behave correctly if you don't follow the same rules as the rest of the
 * library when it comes to setting these fields.
 */
struct oystr {
    size_t len;
    size_t size;
    char *buf;
};

/**
 * Check how much space is available in @s1->buf.
 *
 * The space leftover is the amount of uninitialized memory after
 * s1->buf+s1->len+1.
 */
static inline size_t oystr_avail(struct oystr *s1)
{
    return s1->size ? s1->size - s1->len - 1 : 0;
}

int
oystr_ensure_size(struct oystr *s1, size_t size);

void
oystr_init(struct oystr *s1);

int
oystr_init_buf(struct oystr *s1, size_t size);

void
oystr_deinit(struct oystr *s1);

int
oystr_valid(struct oystr *s1);

int
oystr_set_len(struct oystr *s1, size_t len);

bool
oystr_eq(const struct oystr *s1, const struct oystr *s2);

void
oystr_swap(struct oystr *s1, struct oystr *s2);

char
oystr_trunc(struct oystr *s1, size_t len);

int
oystr_assign(struct oystr *s1, const char *bytes, size_t len);

static inline int
oystr_assign_oystr(struct oystr *s1, const struct oystr *s2)
{
	return oystr_assign(s1, s2->buf, s2->len);
}

int
oystr_write(struct oystr *s1, size_t pos, const char *bytes, size_t len);

static inline int
oystr_write_oystr(struct oystr *s1, size_t pos, const struct oystr *s2)
{
	return oystr_write(s1, pos, s2->buf, s2->len);
}

int
oystr_insert(struct oystr *s1, size_t pos, const char *bytes, size_t len);

static inline int
oystr_insert_oystr(struct oystr *s1, size_t pos, const struct oystr *s2)
{
	return oystr_insert(s1, pos, s2->buf, s2->len);
}

static inline int
oystr_insert_dup(struct oystr *s1, size_t pos)
{
	return oystr_insert(s1, pos, s1->buf, s1->len);
}

int
oystr_append(struct oystr *s1, const char *buf, size_t len);

int
oystr_append_uni(struct oystr *s1, uint32_t wc);

static inline int
oystr_append_oystr(struct oystr *s1, const struct oystr *s2)
{
	return oystr_append(s1, s2->buf, s2->len);
}

static inline int
oystr_append_dup(struct oystr *s1)
{
	return oystr_append(s1, s1->buf, s1->len);
}

void
oystr_slice(struct oystr *slice, struct oystr *s1, size_t begin, size_t end);

bool
oystr_find(struct oystr *slice, struct oystr *s1, const char *buf, size_t len);

size_t
oystr_rstrip(struct oystr *s1, const char *bytes, size_t len);

size_t
oystr_lstrip(struct oystr *s1, const char *bytes, size_t len);

size_t
oystr_strip(struct oystr *s1, const char *bytes, size_t len);

int
oystr_utf8_from_utf32(char *bytes, uint32_t wc);

#endif
