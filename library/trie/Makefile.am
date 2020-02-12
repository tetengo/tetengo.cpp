# Automake Settings
# Copyright (C) 2019 kaoru

SUBDIRS = \
    cpp \
    c \
    list_double_array \
    make_double_array \
    test


iwyu:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} iwyu; cd ..; done


format:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} format; cd ..; done
