# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

SUBDIRS = \
    src

EXTRA_DIST = \
    test_tetengo.cli.vcxproj \
    test_tetengo.cli.vcxproj.filters


iwyu:
	for d in ${SUBDIRS}; do ${MAKE} -C $$d iwyu; done

clean-iwyu:
	for d in ${SUBDIRS}; do ${MAKE} -C $$d clean-iwyu; done


format:
	for d in ${SUBDIRS}; do ${MAKE} -C $$d format; done
