# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

SUBDIRS = \
    src

EXTRA_DIST = \
    README.md \
    search_dict_c.vcxproj \
    search_dict_c.vcxproj.filters


iwyu:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} iwyu; cd ..; done


format:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} format; cd ..; done
