# Automake Settings
# Copyright (C) 2019 kaoru

SUBDIRS = \
    include \
    src

EXTRA_DIST = \
    tetengo-trie.cpp.vcxproj \
    tetengo-trie.cpp.vcxproj.filters


iwyu:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} iwyu; cd ..; done


format:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} format; cd ..; done
