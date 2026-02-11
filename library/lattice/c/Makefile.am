# Automake Settings
# $12026 kaoru  https://www.tetengo.org/

SUBDIRS = \
    include \
    src

EXTRA_DIST = \
    tetengo.lattice.vcxproj \
    tetengo.lattice.vcxproj.filters


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
