# Automake Settings
# Copyright (C) 2019 kaoru

SUBDIRS = \
    src

EXTRA_DIST = \
    tetengo-trie.test.vcxproj \
    tetengo-trie.test.vcxproj.filters


.PHONY: format
format:
	for d in ${SUBDIRS}; \
	do \
	    cd $$d; \
	    ${MAKE} format; \
	    cd ..; \
	done
