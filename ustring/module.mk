SRC += $(patsubst %, ustring/%, \
       internal.h \
       ustr_alloc.c \
       ustr_cat.c \
       ustr_cpy.c \
       ustr_dup.c \
       ustr_ensure_size.c \
       ustr_eq.c \
       ustr_find.c \
       ustr_free.c \
       ustr_grow.c \
       ustr_ins.c \
       ustr_slice.c \
       ustr_strip.c \
       ustr_tok.c \
       ustr_trunc.c \
       ustr_utf.c)
