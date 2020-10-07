# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

SUBDIRS = \
    src

EXTRA_DIST = \
    README.md \
    search_dict_c.vcxproj \
    search_dict_c.vcxproj.filters


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
