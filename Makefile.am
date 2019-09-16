# Automake Settings
# Copyright (C) 2019 kaoru

SUBDIRS = \
    docsrc \
    precompiled \
    tools \
    trie \
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
