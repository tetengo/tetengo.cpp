# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo_trie_trie.cpp \
    tetengo_trie_trieIterator.cpp

lib_LTLIBRARIES = libtetengo.trie.la

libtetengo_trie_la_CPPFLAGS = \
    -I${top_srcdir}/library/trie/c/include \
    -I${top_srcdir}/library/trie/cpp/include
libtetengo_trie_la_LIBADD = \
    ${top_builddir}/library/trie/cpp/src/libtetengo.trie.noinst.la
libtetengo_trie_la_SOURCES = ${sources}

EXTRA_DIST = \
    dll_exports.def


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${libtetengo_trie_la_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

clean-local:
	-find -name "*.iwyuout" | xargs rm -f


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
