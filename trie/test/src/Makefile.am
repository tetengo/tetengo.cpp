# Automake Settings
# Copyright (C) 2019 kaoru

headers =

sources = \
    master.cpp \
    test_tetengo.trie.double_array.cpp

check_PROGRAMS = test_tetengo.trie

test_tetengo_trie_CPPFLAGS = \
    -I${top_srcdir}/trie/c/include \
    -I${top_srcdir}/trie/cpp/include
test_tetengo_trie_LDFLAGS = \
    -L${top_srcdir}/trie/c/src \
    -L${top_srcdir}/trie/cpp/src
test_tetengo_trie_LDADD = \
    -ltetengo.trie.c \
    -ltetengo.trie.cpp \
    ${BOOST_UNIT_TEST_FRAMEWORK_LIB}
test_tetengo_trie_SOURCES = ${sources}

TESTS = ${check_PROGRAMS}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${test_tetengo_trie_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

clean-local:
	-find -name "*.iwyuout" | xargs rm -f


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
