# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    master.cpp \
    test_tetengo.trie.default_serializer.cpp \
    test_tetengo.trie.double_array.cpp \
    test_tetengo.trie.double_array_iterator.cpp \
    test_tetengo.trie.memory_storage.cpp \
    test_tetengo.trie.shared_storage.cpp \
    test_tetengo.trie.storage.cpp \
    test_tetengo.trie.trie.cpp \
    test_tetengo.trie.trie_iterator.cpp

check_PROGRAMS = test_tetengo.trie

test_tetengo_trie_CPPFLAGS = \
    -I${top_srcdir}/library/trie/c/include \
    -I${top_srcdir}/library/trie/cpp/include
test_tetengo_trie_LDFLAGS = \
    -L${top_builddir}/library/trie/c/src
test_tetengo_trie_LDADD = \
    -ltetengo.trie \
    ${BOOST_UNIT_TEST_FRAMEWORK_LIB}
test_tetengo_trie_DEPENDENCIES = \
    ${top_builddir}/library/trie/c/src/libtetengo.trie.la
test_tetengo_trie_SOURCES = ${sources}

TESTS = ${check_PROGRAMS}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_trie_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
