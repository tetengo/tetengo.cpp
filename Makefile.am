# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

ACLOCAL_AMFLAGS = -I m4

SOURCE_SUBDIRS = \
    library \
    executable

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


.PHONY: precompiled_h
precompiled_h:
	cd $(top_srcdir) && ./tools/make_precompiled_h.py

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
	for d in ${SOURCE_SUBDIRS}; do ${MAKE} -C $$d iwyu; done
	for f in $$(find -name "*.iwyuout"); do ${top_srcdir}/tools/filter_iwyuout.py < $$f; done

clean-iwyu:
	for d in ${SOURCE_SUBDIRS}; do ${MAKE} -C $$d clean-iwyu; done


format:
	for d in ${SOURCE_SUBDIRS}; do ${MAKE} -C $$d format; done
