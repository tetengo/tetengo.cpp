# Automake Settings
# Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/

SOURCE_SUBDIRS = \
    src

SUBDIRS = \
    ${SOURCE_SUBDIRS} \
    data

data_files = \
    README.md

EXTRA_DIST = \
    ${data_files} \
    transfer_trains.vcxproj \
    transfer_trains.vcxproj.filters

transfer_trains_datadir = ${pkgdatadir}/transfer_trains

transfer_trains_data_DATA = ${data_files}


iwyu: ${SOURCE_SUBDIRS}

clean-iwyu: ${SOURCE_SUBDIRS}

format: ${SOURCE_SUBDIRS}

clean-format: ${SOURCE_SUBDIRS}

.PHONY: ${SOURCE_SUBDIRS}
${SOURCE_SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
