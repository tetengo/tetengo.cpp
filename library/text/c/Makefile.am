# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

SUBDIRS = \
    include \
    src

EXTRA_DIST = \
    tetengo.text.vcxproj \
    tetengo.text.vcxproj.filters


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
