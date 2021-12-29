# Automake Settings
# Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/

SOURCE_SUBDIRS = \
    src

SUBDIRS = \
    ${SOURCE_SUBDIRS} \
    data

data_files = \
    README.md

EXTRA_DIST = \
    ${data_files} \
    json2timetable.vcxproj \
    json2timetable.vcxproj.filters

json2timetable_datadir = ${pkgdatadir}/json2timetable

json2timetable_data_DATA = ${data_files}


iwyu: ${SOURCE_SUBDIRS}

clean-iwyu: ${SOURCE_SUBDIRS}

format: ${SOURCE_SUBDIRS}

clean-format: ${SOURCE_SUBDIRS}

.PHONY: ${SOURCE_SUBDIRS}
${SOURCE_SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
