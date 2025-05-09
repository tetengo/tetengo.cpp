# Automake Settings
# Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo.trie.default_serializer.cpp \
    tetengo.trie.double_array.cpp \
    tetengo.trie.double_array_builder.cpp \
    tetengo.trie.double_array_builder.hpp \
    tetengo.trie.double_array_iterator.cpp \
    tetengo.trie.memory_storage.cpp \
    tetengo.trie.mmap_storage.cpp \
    tetengo.trie.shared_storage.cpp \
    tetengo.trie.storage.cpp \
    tetengo.trie.trie.cpp\
    tetengo.trie.trie_iterator.cpp \
    tetengo.trie.value_serializer.cpp

lib_LIBRARIES = libtetengo.trie.cpp.a

libtetengo_trie_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/library/trie/cpp/include
libtetengo_trie_cpp_a_SOURCES = ${headers} ${sources}

noinst_LTLIBRARIES = libtetengo.trie.noinst.la

libtetengo_trie_noinst_la_CPPFLAGS = ${libtetengo_trie_cpp_a_CPPFLAGS}
libtetengo_trie_noinst_la_CXXFLAGS = -fPIC
libtetengo_trie_noinst_la_LDFLAGS = -static
libtetengo_trie_noinst_la_SOURCES = ${libtetengo_trie_cpp_a_SOURCES}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_trie_cpp_a_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


format: ${addsuffix .formatout, ${headers} ${sources}}

%.formatout: %
	CLANGFORMAT=${CLANGFORMAT} DOS2UNIX=${DOS2UNIX} ${top_srcdir}/kogyan/tool/call_clang-format.sh $< || true
	${MKDIR_P} ${dir $@}
	touch $@

.PHONY: clean-format
clean-format:
	-find -name "*.formatout" | xargs rm -f

clean-local: clean-format
