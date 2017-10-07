/* See LICENSE file for copyright and license details */
#define USTR(str) (Ustring){(str), strlen(str)}

typedef struct {
	uint8_t *mem;
	size_t len;
} Ustr;

typedef struct {
	uint8_t *mem;
	size_t len;
	size_t size;
} Ustring;

static inline Ustr *
ustr_clear(Ustr *sp)
{
	sp->len = 0;
	return sp;
}

static inline size_t
ustr_avail(Ustring *sp)
{
	return sp->size - sp->len;
}

static inline bool
ustr_valid(Ustring *sp)
{
	if (!sp || !sp->mem || sp->size < sp->len)
		return false;
	return true;
}

static inline void
ustr_term(Ustring *sp)
{
	sp->mem[sp->len] = '\0';
}

int ustr_grow(Ustring *sp, size_t n);
int ustr_ensure_size(Ustring *sp, size_t n);
int ustr_alloc(Ustring *sp, size_t n);
void ustr_free(Ustring const *sp);
bool ustr_eq(Ustr const *s1, Ustr const *s2);
Ustr *ustr_trunc(Ustr *sp, size_t n);
int ustr_dup_mem(Ustring *dest, void const *src, size_t n);
int ustr_dup_cstr(Ustring *dest, char const *src);
int ustr_dup(Ustring *dest, Ustr const *src);
int ustr_cpy(Ustring *dest, Ustr const *src);
int ustr_cpy_mem(Ustring *dest, void const *src, size_t n);
int ustr_cpy_cstr(Ustring *dest, char const *src);
int ustr_ins(Ustring *dest, size_t pos, Ustr const *src);
int ustr_ins_mem(Ustring *dest, size_t pos, void const *src, size_t n);
int ustr_ins_cstr(Ustring *dest, size_t pos, char const *src);
int ustr_ins_utf8f32(Ustring *dest, size_t pos, uint32_t wc);
int ustr_cat(Ustring *dest, Ustr const *src);
int ustr_cat_byte(Ustring *dest, uint8_t src);
int ustr_cat_mem(Ustring *dest, void const *src, size_t n);
int ustr_cat_cstr(Ustring *dest, char const *src);
int ustr_cat_utf8f32(Ustring *dest, uint32_t src);
bool ustr_find_mem(Ustr *dest, Ustr const *haystack, void const *needle, size_t n);
bool ustr_find_cstr(Ustr *dest, Ustr const *haystack, char const *needle);
bool ustr_find(Ustr *dest, Ustr const *haystack, Ustr const *needle);
void ustr_slice(Ustr *dest, Ustr const *src, size_t begin, size_t end);
Ustring *ustr_rstrip_mem(Ustring *sp, void const *chs, size_t n);
Ustring *ustr_lstrip_mem(Ustring *sp, void const *chs, size_t n);
Ustring *ustr_strip_mem(Ustring *sp, void const *chs, size_t n);
Ustring *ustr_rstrip_cstr(Ustring *sp, char const *chs);
Ustring *ustr_lstrip_cstr(Ustring *sp, char const *chs);
Ustring *ustr_strip_cstr(Ustring *sp, char const *chs);
Ustring *ustr_rstrip(Ustring *sp, Ustr const *chs);
Ustring *ustr_lstrip(Ustring *sp, Ustr const *chs);
Ustring *ustr_strip(Ustring *sp, Ustr const *chs);
bool ustr_tok_mem(Ustr *dest, Ustr *pos, void const *delim, size_t n);
bool ustr_tok_cstr(Ustr *dest, Ustr *pos, char const *delim);
bool ustr_tok(Ustr *dest, Ustr *pos, Ustr const *delim);
size_t ustr_utf8len(Ustr const *sp);
size_t ustr_utf8n32(uint32_t wc);
size_t ustr_utf8f32(void *dest, uint32_t wc);
