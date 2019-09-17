# Automake Settings
# Copyright (C) 2019 kaoru

headers =

sources = \
    tetengo.trie.double_array.cpp

lib_LIBRARIES = libtetengo.trie.c.a

libtetengo_trie_c_a_CPPFLAGS = \
    -I${top_srcdir}/trie/c/include \
    -I${top_srcdir}/trie/cpp/include
libtetengo_trie_c_a_SOURCES = ${sources}

EXTRA_DIST = \
    dll_exports.def


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${libtetengo_trie_c_a_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

clean-local:
	-find -name "*.iwyuout" | xargs rm -f


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
