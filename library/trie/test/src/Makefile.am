# Automake Settings
# Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/

headers = \
    usage_tetengo.trie.search_c.h \
    usage_tetengo.trie.search_cpp.hpp

sources = \
    master.cpp \
    test_tetengo.trie.default_serializer.cpp \
    test_tetengo.trie.double_array.cpp \
    test_tetengo.trie.double_array_iterator.cpp \
    test_tetengo.trie.memory_storage.cpp \
    test_tetengo.trie.mmap_storage.cpp \
    test_tetengo.trie.shared_storage.cpp \
    test_tetengo.trie.storage.cpp \
    test_tetengo.trie.trie.cpp \
    test_tetengo.trie.trie_iterator.cpp \
    test_tetengo.trie.value_serializer.cpp \
    usage_tetengo.trie.cpp \
    usage_tetengo.trie.search_c.c \
    usage_tetengo.trie.search_cpp.cpp

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
test_tetengo_trie_SOURCES = ${headers} ${sources}

TESTS = ${check_PROGRAMS}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.c.iwyuout: %.c
	${IWYU} ${IWYU_OPTS_C} ${CPPFLAGS} ${test_tetengo_trie_CPPFLAGS} ${CFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.h.iwyuout: %.h
	${IWYU} ${IWYU_OPTS_C} ${CPPFLAGS} ${test_tetengo_trie_CPPFLAGS} ${CFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.cpp.iwyuout: %.cpp
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_trie_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.hpp.iwyuout: %.hpp
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_trie_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
