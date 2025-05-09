# Automake Settings
# Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/

headers = \
    usage_tetengo.lattice.viterbi_c.h \
    usage_tetengo.lattice.viterbi_cpp.hpp

sources = \
    master.cpp \
    test_tetengo.lattice.connection.cpp \
    test_tetengo.lattice.constraint.cpp \
    test_tetengo.lattice.constraint_element.cpp \
    test_tetengo.lattice.custom_input.cpp \
    test_tetengo.lattice.custom_vocabulary.cpp \
    test_tetengo.lattice.entry.cpp \
    test_tetengo.lattice.input.cpp \
    test_tetengo.lattice.lattice.cpp \
    test_tetengo.lattice.n_best_iterator.cpp \
    test_tetengo.lattice.node.cpp \
    test_tetengo.lattice.node_constraint_element.cpp \
    test_tetengo.lattice.path.cpp \
    test_tetengo.lattice.string_input.cpp \
    test_tetengo.lattice.string_view.cpp \
    test_tetengo.lattice.unordered_map_vocabulary.cpp \
    test_tetengo.lattice.vocabulary.cpp \
    test_tetengo.lattice.wildcard_constraint_element.cpp \
    usage_tetengo.lattice.cpp \
    usage_tetengo.lattice.viterbi_c.c \
    usage_tetengo.lattice.viterbi_cpp.cpp

check_PROGRAMS = test_tetengo.lattice

test_tetengo_lattice_CPPFLAGS = \
    -I${top_srcdir}/library/lattice/c/include \
    -I${top_srcdir}/library/lattice/cpp/include
test_tetengo_lattice_LDFLAGS = \
    -L${top_builddir}/library/lattice/c/src
test_tetengo_lattice_LDADD = \
    -ltetengo.lattice \
    ${BOOST_UNIT_TEST_FRAMEWORK_LIB}
test_tetengo_lattice_DEPENDENCIES = \
    ${top_builddir}/library/lattice/c/src/libtetengo.lattice.la
test_tetengo_lattice_SOURCES = ${headers} ${sources}

TESTS = ${check_PROGRAMS}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.c.iwyuout: %.c
	${IWYU} ${IWYU_OPTS_C} ${CPPFLAGS} ${test_tetengo_lattice_CPPFLAGS} ${CFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.h.iwyuout: %.h
	${IWYU} ${IWYU_OPTS_C} ${CPPFLAGS} ${test_tetengo_lattice_CPPFLAGS} ${CFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.cpp.iwyuout: %.cpp
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_lattice_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.hpp.iwyuout: %.hpp
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_lattice_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
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
