# Automake Settings
# Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

script_files = \
    make_character_property_map.py \
    make_character_property_map_cpp.py

EXTRA_DIST = \
    ${script_files} \
    install_tetengo_library.bat \
    make_character_property_map_cpp.bat \
    make_character_property_map_cpp.sh


format: ${script_files}
	${BLACK} ${srcdir}
	${ISORT} ${srcdir}
