# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

SUBDIRS = \
    include \
    src

EXTRA_DIST = \
    tetengo.cli.vcxproj \
    tetengo.cli.vcxproj.filters


iwyu:
	for d in ${SUBDIRS}; do ${MAKE} -C $$d iwyu; done

clean-iwyu:
	for d in ${SUBDIRS}; do ${MAKE} -C $$d clean-iwyu; done


format:
	for d in ${SUBDIRS}; do ${MAKE} -C $$d format; done
