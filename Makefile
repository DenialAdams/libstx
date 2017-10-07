# See LICENSE file for copyright and license details.
# Project configuration
include config.mk

LDFLAGS += -Iinclude

MODULES := ustring math io
LIBS :=
SRC := \
	include/ustring.h \
	include/ustring/math.h \
	include/ustring/io.h

# Include project modules
include $(patsubst %, %/module.mk, $(MODULES))

_OBJ := $(patsubst %.c, %.o, $(filter %.c, $(SRC)))
OBJ := $(patsubst %, build/%, $(_OBJ))
OBJ_DEBUG := $(patsubst %.o, build/%_d.o, $(_OBJ))

# Standard targets
all: options release

options: config.mk
	@printf -- "ustring build options:\n"
	@printf -- "CFLAGS  = %s\n" "$(CFLAGS)"
	@printf -- "LDFLAGS = %s\n" "$(LDFLAGS)"
	@printf -- "CC      = %s\n" "$(CC)"

dist: clean

clean:
	@printf "Cleaning\n"
	@rm -rf build

# Release build
build/%.o: %.c
	@printf "CC [R] $@\n"
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

# Debug build
build/%_d.o: %.c
	@printf "CC [D] $@\n"
	@mkdir -p $(shell dirname $@)
	@$(CC) -g $(CFLAGS) $(LDFLAGS) -c -o $@ $<

# Release target
build/ustring.a: $(OBJ)
	@printf "Creating library archive for release... "
	@ar -cq $@ $?
	@printf "done.\n"

# Debug target
build/ustring_d.a: $(OBJ_DEBUG)
	@printf "Creating library archive for debugging... "
	@ar -cq $@ $?
	@printf "done.\n"

release: build/ustring.a

debug: build/ustring_d.a

# Build tests
test:

.PHONY: all options check clean dist install uninstall
