# Automake Settings
# Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/

SUBDIRS = \
    src

data_files = \
    README.md

EXTRA_DIST = \
    ${data_files} \
    make_dict.vcxproj \
    make_dict.vcxproj.filters

make_dict_datadir = ${pkgdatadir}/make_dict

make_dict_data_DATA = ${data_files}


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
