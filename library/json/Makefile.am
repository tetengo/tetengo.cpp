# Automake Settings
# $12026 kaoru  https://www.tetengo.org/

SUBDIRS = \
    cpp \
    c \
    test


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
