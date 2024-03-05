ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

# directories
DATAROOTDIR := $(PREFIX)/share
MANDIR      := $(DATAROOTDIR)/man
MAN3DIR     := $(MANDIR)/man3

EXEC_PREFIX := $(PREFIX)
LIBDIR      := $(EXEC_PREFIX)/lib

INCLUDEDIR := $(PREFIX)/include

# install command
INSTALL         := install -D
INSTALL_PROGRAM := $(INSTALL)
INSTALL_DATA    := $(INSTALL) -m 664

# remove command
REMOVE      := rm -fv
REMOVE_FILE := $(REMOVE)

.PHONY: install uninstall

install: build-static build-shared
	$(INSTALL_PROGRAM) bin/libcliscreen.so\
		$(DESTDIR)$(LIBDIR)/libcliscreen.so
	$(INSTALL_DATA) bin/libcliscreen.a\
		$(DESTDIR)$(LIBDIR)/libcliscreen.a
	$(INSTALL_DATA) include/cliscreen.h\
		$(DESTDIR)$(INCLUDEDIR)/cliscreen.h
	# TODO manpages (add to the list of dependencies as well)

uninstall:
	$(REMOVE_FILE) $(DESTDIR)$(LIBDIR)/libcliscreen.so
	$(REMOVE_FILE) $(DESTDIR)$(LIBDIR)/libcliscreen.a
	$(REMOVE_FILE) $(DESTDIR)$(INCLUDEDIR)/cliscreen.h
	# TODO manpages
