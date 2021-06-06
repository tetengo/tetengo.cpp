# Automake Settings
# Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

SUBDIRS = \
    cpp \
    c


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
