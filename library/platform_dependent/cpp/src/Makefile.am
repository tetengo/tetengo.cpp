# Automake Settings
# Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/

IWYU_SUBDIRS = \
    unixos

SUBDIRS = \
    ${IWYU_SUBDIRS} \
    windows


iwyu: ${IWYU_SUBDIRS}

clean-iwyu: ${IWYU_SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
