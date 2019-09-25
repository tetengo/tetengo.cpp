# Automake Settings
# Copyright (C) 2019 kaoru

headers =

sources = \
    tetengo.trie.double_array.cpp \
    tetengo.trie.double_array_builder.cpp \
    tetengo.trie.double_array_builder.hpp

lib_LIBRARIES = libtetengo.trie.cpp.a

libtetengo_trie_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/trie/cpp/include
libtetengo_trie_cpp_a_SOURCES = ${sources}

noinst_LTLIBRARIES = libtetengo.trie.noinst.la

libtetengo_trie_noinst_la_CPPFLAGS = ${libtetengo_trie_cpp_a_CPPFLAGS}
libtetengo_trie_noinst_la_CXXFLAGS = -fPIC
libtetengo_trie_noinst_la_LDFLAGS = -static
libtetengo_trie_noinst_la_SOURCES = ${libtetengo_trie_cpp_a_SOURCES}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${libtetengo_trie_cpp_a_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

clean-local:
	-find -name "*.iwyuout" | xargs rm -f


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
