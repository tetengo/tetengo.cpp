# Automake Settings
# Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/

script_files = \
    copy_libimage_files.py \
    generate_content_wxs.py \
    generate_content_wxs_source.py

EXTRA_DIST = \
    ${script_files} \
    COPYING.rtf \
    envvars_to_install.txt \
    file_guid_map.txt \
    files_to_install.txt \
    installer.vcxproj \
    installer.vcxproj.filters \
    main.wxs \
    Makefile.nmake


mypy: ${script_files}
	${MYPY} ${MYPY_OPTS} ${srcdir}

format: ${script_files}
	${BLACK} ${srcdir}
	${ISORT} ${srcdir}
