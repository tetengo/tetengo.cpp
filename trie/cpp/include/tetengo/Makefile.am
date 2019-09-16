# Automake Settings
# Copyright (C) 2019 kaoru

pkg_headers = \
    trie/double_array.hpp

extra_headers = \
    trie/0namespace.dox

nobase_pkginclude_HEADERS = ${pkg_headers}

EXTRA_DIST = ${extra_headers}


.PHONY: format
format:
	for f in ${pkg_headers} ${extra_headers}; \
	do \
	    ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; \
	done
