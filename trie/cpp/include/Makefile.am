# Automake Settings
# Copyright (C) 2019 kaoru

SUBDIRS = \
    tetengo


format:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} format; cd ..; done
