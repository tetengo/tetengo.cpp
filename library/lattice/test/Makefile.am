# Automake Settings
# Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/

SUBDIRS = \
    src

EXTRA_DIST = \
    test_tetengo.lattice.vcxproj \
    test_tetengo.lattice.vcxproj.filters


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
