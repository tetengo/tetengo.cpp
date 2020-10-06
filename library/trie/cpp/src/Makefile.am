# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo.trie.default_serializer.cpp \
    tetengo.trie.double_array.cpp \
    tetengo.trie.double_array_builder.cpp \
    tetengo.trie.double_array_builder.hpp \
    tetengo.trie.double_array_iterator.cpp \
    tetengo.trie.memory_storage.cpp \
    tetengo.trie.shared_storage.cpp \
    tetengo.trie.storage.cpp \
    tetengo.trie.trie.cpp\
    tetengo.trie.trie_iterator.cpp

lib_LIBRARIES = libtetengo.trie.cpp.a

libtetengo_trie_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/library/trie/cpp/include
libtetengo_trie_cpp_a_SOURCES = ${headers} ${sources}

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

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
