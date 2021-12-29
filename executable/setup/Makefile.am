# Automake Settings
# Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/

SOURCE_SUBDIRS = \
    src

SUBDIRS = \
    ${SOURCE_SUBDIRS} \
    res

EXTRA_DIST = \
    setup.vcxproj \
    setup.vcxproj.filters


iwyu: ${SOURCE_SUBDIRS}

clean-iwyu: ${SOURCE_SUBDIRS}

format: ${SOURCE_SUBDIRS}

clean-format: ${SOURCE_SUBDIRS}

.PHONY: ${SOURCE_SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
