# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

SUBDIRS = \
    include \
    src

EXTRA_DIST = \
    tetengo.json.cpp.vcxproj \
    tetengo.json.cpp.vcxproj.filters


iwyu:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} iwyu; cd ..; done

clean-iwyu:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} clean-iwyu; cd ..; done


format:
	for d in ${SUBDIRS}; do cd $$d; ${MAKE} format; cd ..; done
