# Automake Settings
# Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/

script_files = \
    make_character_property_map.py \
    make_character_property_map_cpp.py

EXTRA_DIST = \
    ${script_files} \
    make_character_property_map_cpp.bat \
    make_character_property_map_cpp.sh


mypy: ${script_files}
	${MYPY} ${MYPY_OPTS} ${srcdir}

format: ${script_files}
	${BLACK} ${srcdir}
	${ISORT} ${ISORT_OPTS} ${srcdir}
