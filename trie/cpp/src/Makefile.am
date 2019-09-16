# Automake Settings
# Copyright (C) 2019 kaoru

headers =

sources = \
    tetengo.trie.double_array.cpp

lib_LIBRARIES = libtetengo.trie.cpp.a

libtetengo_trie_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/trie/cpp/include
libtetengo_trie_cpp_a_SOURCES = ${sources}


.PHONY: format
format:
	for f in ${headers} ${sources}; \
	do \
	    ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; \
	done
