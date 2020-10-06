# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

SOURCE_SUBDIRS = \
    src

SUBDIRS = \
    ${SOURCE_SUBDIRS} \
    data

EXTRA_DIST = \
    README.md \
    transfer_trains.vcxproj \
    transfer_trains.vcxproj.filters


iwyu:
	for d in ${SOURCE_SUBDIRS}; do ${MAKE} -C $$d iwyu; done

clean-iwyu:
	for d in ${SOURCE_SUBDIRS}; do ${MAKE} -C $$d clean-iwyu; done


format:
	for d in ${SOURCE_SUBDIRS}; do ${MAKE} -C $$d format; done
