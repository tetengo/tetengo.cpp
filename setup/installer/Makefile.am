# Automake Settings
# Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

script_files = \
    generate_content_wxs.py \
    generate_content_wxs_source.py

EXTRA_DIST = \
    ${script_files} \
    COPYING.rtf \
    file_guid_map.txt \
    files_to_install.txt \
    installer.vcxproj \
    installer.vcxproj.filters \
    main.wxs \
    Makefile.nmake


mypy: ${script_files}
	${MYPY} ${srcdir}

format: ${script_files}
	${BLACK} ${srcdir}
	${ISORT} ${srcdir}
