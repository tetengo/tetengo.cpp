# Automake Settings
# Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/

SUBDIRS = \
    json \
    lattice \
    platform_dependent \
    text \
    property \
    trie


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
