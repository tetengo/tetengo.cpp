# Automake Settings
# Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/

SUBDIRS = \
    cpp \
    test


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
