# Automake Settings
# Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/

SUBDIRS = \
    include \
    src

EXTRA_DIST = \
    tetengo.platform_dependent.cpp.vcxproj \
    tetengo.platform_dependent.cpp.vcxproj.filters


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
