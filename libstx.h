/* See LICENSE file for copyright and license details */
/**
 * Dynamic and modifiable string data structure. Contents are modifiable and
 * contains both the size of the memory, and how much is being used.
 */
typedef struct {
	size_t len;
	uint8_t *mem;
	size_t size;
} String;

/**
 * String slice data structure. Contains a read-only reference to a character
 * array and the length of the character array. Created by calling stxref() on
 * a struct stx.
 */
typedef struct {
	size_t len;
	uint8_t const *mem;
} Str;

bool stx_eq(Str const *a, Str const *b);
bool stx_cat(String *dst, Str const *src);
void stx_trunc(Str *a, size_t n);
size_t stx_avail(String *a);
static inline bool
stx_valid(String *a)
{
//TODO
}
static inline void
stx_clear(Str *a);
{
	sp->len = 0;
	return sp;
}
// Copy bytes from "src" into a stx.
String *stxcpy(String *dst, Str const *src);
String *stxcpy_mem(String *sp, void const *src, size_t n);
String *stxcpy_cstr(stx *sp, char const *src);

// Insert bytes into the middle of stx without overwriting any data.
String *stxins(String *dst, size_t pos, Str const *src);
String *stxins_mem(stx *sp, size_t pos, const void *src, size_t n);
String *stxins_cstr(stx *sp, size_t pos, const char *src);
String *stxins_utf8f32(stx *sp, size_t pos, uint32_t wc);

// Append bytes to a stx.
String *stxapp(String *dst, Str const *src);
String *stxapp_byte(stx *sp, const uint8_t src);
String *stxapp_mem(stx *sp, const void *src, size_t n);
String *stxapp_cstr(stx *sp, const char *src);
String *stxapp_utf8f32(stx *sp, uint32_t wc);

//TODO(todd): Revise prototypes below
// Find a substring inside a stx and return it as a spx referring to it.
spx stxfind_mem(const spx haystack, const void *needle, size_t n);
spx stxfind_str(const spx haystack, const char *needle);
spx stxfind_spx(const spx haystack, const spx needle);

// Slice a substring inside a spx and return it as a spx referring to it.
spx stxslice(const spx sp, size_t begin, size_t end);

// Strip characters from a stx.
stx *stxrstrip(stx *sp, const void *chs, size_t n);
stx *stxlstrip(stx *sp, const void *chs, size_t n);
stx *stxstrip(stx *sp, const void *chs, size_t n);

// Tokenize a spx.
spx stxtok(spx *sp, const char *chs, size_t n);

// Calculate the number of utf8 encodings in a spx.
size_t stxutf8len(const spx sp);
// Calculate the number of bytes for a utf8 encoding of a utf32 code point.
size_t stxutf8n32(uint32_t wc);
// Convert a "wc" into a utf8 encoding and store it in "dest".
size_t stxutf8f32(void *dest, uint32_t wc);

#endif
