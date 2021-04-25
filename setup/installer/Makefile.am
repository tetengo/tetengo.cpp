# Automake Settings
# Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

script_files = \
    generate_content_wxs_source.py

EXTRA_DIST =


mypy: ${script_files}
	${MYPY} ${srcdir}

format: ${script_files}
	${BLACK} ${srcdir}
	${ISORT} ${srcdir}
