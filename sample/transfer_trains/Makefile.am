# Automake Settings
# Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

SOURCE_SUBDIRS = \
    src

SUBDIRS = \
    ${SOURCE_SUBDIRS} \
    data

EXTRA_DIST = \
    README.md \
    transfer_trains.vcxproj \
    transfer_trains.vcxproj.filters


iwyu: ${SOURCE_SUBDIRS}

clean-iwyu: ${SOURCE_SUBDIRS}

format: ${SOURCE_SUBDIRS}

clean-format: ${SOURCE_SUBDIRS}

.PHONY: ${SOURCE_SUBDIRS}
${SOURCE_SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
