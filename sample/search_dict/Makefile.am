# Automake Settings
# Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/

SUBDIRS = \
    src

data_files = \
    README.md

EXTRA_DIST = \
    ${data_files} \
    search_dict.vcxproj \
    search_dict.vcxproj.filters

search_dict_datadir = ${pkgdatadir}/search_dict

search_dict_data_DATA = ${data_files}


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
