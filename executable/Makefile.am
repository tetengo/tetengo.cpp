# Automake Settings
# Copyright (C) 2019 kaoru

SUBDIRS = \
    list_double_array \
    make_double_array


iwyu:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} iwyu; cd ..; done


format:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} format; cd ..; done
