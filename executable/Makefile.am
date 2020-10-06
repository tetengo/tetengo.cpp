# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

SUBDIRS = \
    make_dict \
    search_dict \
    search_dict_c \
    transfer_trains


iwyu:
	for d in ${SUBDIRS}; do ${MAKE} -C $$d iwyu; done

clean-iwyu:
	for d in ${SUBDIRS}; do ${MAKE} -C $$d clean-iwyu; done


format:
	for d in ${SUBDIRS}; do ${MAKE} -C $$d format; done
