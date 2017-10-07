/* See LICENSE file for copyright and license details */
size_t ustr_printf(const char *fmt, ...);
size_t ustr_fprintf(FILE *stream, const char *fmt, ...);
size_t ustr_fputs(FILE *stream, Ustring const *sp);
Ustring *ustr_fgets(Ustring *sp, FILE *stream);
