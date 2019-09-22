# Automake Settings
# Copyright (C) 2019 kaoru

ACLOCAL_AMFLAGS = -I m4

SOURCE_SUBDIRS = \
    trie

SUBDIRS = \
    ${SOURCE_SUBDIRS} \
    docsrc \
    precompiled \
    tools \
    vsprops

EXTRA_DIST = \
    bootstrap.sh \
    Doxyfile \
    README.md \
    tetengo.sln


.PHONY: doc
doc:
	cd $(top_srcdir) && $(DOXYGEN) Doxyfile
	if test "$(top_srcdir)" != "$(top_builddir)"; \
	then \
	    rm -rf $(top_builddir)/doc; \
	    mv $(top_srcdir)/doc $(top_builddir); \
	fi

.PHONY: clean-doc
clean-doc:
	-rm -rf $(top_builddir)/doc

distclean-local: clean-doc


iwyu:
	find -type f -name *.iwyuout -print0 | xargs -0 rm -f
	for d in ${SOURCE_SUBDIRS}; do cd $$d; ${MAKE} iwyu; cd ..; done
	for f in ${shell find -name "*.iwyuout"}; do ${top_srcdir}/tools/filter_iwyuout.py < $$f; done


format:
	for d in ${SOURCE_SUBDIRS}; do cd $$d; ${MAKE} format; cd ..; done
