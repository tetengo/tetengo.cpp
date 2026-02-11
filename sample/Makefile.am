# Automake Settings
# Copyright (C) 2019-2026 kaoru  https://www.tetengo.org/

SUBDIRS = \
    json2timetable \
    make_dict \
    search_dict \
    search_dict_c \
    transfer_trains


iwyu: ${SUBDIRS}

clean-iwyu: ${SUBDIRS}

format: ${SUBDIRS}

clean-format: ${SUBDIRS}

.PHONY: ${SUBDIRS}
${SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}
