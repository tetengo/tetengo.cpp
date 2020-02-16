# Automake Settings
# Copyright (C) 2019-2020 kaoru

SUBDIRS = \
    cpp \
    c \
    test


iwyu:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} iwyu; cd ..; done


format:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} format; cd ..; done
