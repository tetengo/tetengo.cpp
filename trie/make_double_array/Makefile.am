# Automake Settings
# Copyright (C) 2019 kaoru

SUBDIRS = \
    src

EXTRA_DIST = \
    make_double_array.vcxproj \
    make_double_array.vcxproj.filters


iwyu:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} iwyu; cd ..; done


format:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} format; cd ..; done
