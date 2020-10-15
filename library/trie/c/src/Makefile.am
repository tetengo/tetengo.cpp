# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers = \
    tetengo_trie_storage.hpp \
    tetengo_trie_trie.hpp

sources = \
    tetengo_trie_storage.cpp \
    tetengo_trie_trie.cpp \
    tetengo_trie_trieIterator.cpp

lib_LTLIBRARIES = libtetengo.trie.la

libtetengo_trie_la_CPPFLAGS = \
    -I${top_srcdir}/library/trie/c/include \
    -I${top_srcdir}/library/trie/cpp/include
libtetengo_trie_la_LIBADD = \
    ${top_builddir}/library/trie/cpp/src/libtetengo.trie.noinst.la
libtetengo_trie_la_SOURCES = ${headers} ${sources}

EXTRA_DIST = \
    dll_exports.def


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_trie_la_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


format: ${addsuffix .formatout, ${headers} ${sources}}

%.formatout: %
	${top_srcdir}/tools/call_clang-format.sh $< || true
	${MKDIR_P} ${dir $@}
	touch $@

.PHONY: clean-format
clean-format:
	-find -name "*.formatout" | xargs rm -f

clean-local: clean-format
