# Automake Settings
# Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

SUBDIRS = \
    src

EXTRA_DIST = \
    README.md \
    make_dict.vcxproj \
    make_dict.vcxproj.filters


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
