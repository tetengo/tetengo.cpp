# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    master.cpp \
    test_tetengo.lattice.connection.cpp \
    test_tetengo.lattice.constraint.cpp \
    test_tetengo.lattice.constraint_element.cpp \
    test_tetengo.lattice.entry.cpp \
    test_tetengo.lattice.lattice.cpp \
    test_tetengo.lattice.n_best_iterator.cpp \
    test_tetengo.lattice.node.cpp \
    test_tetengo.lattice.node_constraint_element.cpp \
    test_tetengo.lattice.path.cpp \
    test_tetengo.lattice.string_view.cpp \
    test_tetengo.lattice.unordered_map_vocabulary.cpp \
    test_tetengo.lattice.vocabulary.cpp \
    test_tetengo.lattice.wildcard_constraint_element.cpp

check_PROGRAMS = test_tetengo.lattice

test_tetengo_lattice_CPPFLAGS = \
    -I${top_srcdir}/library/lattice/c/include \
    -I${top_srcdir}/library/lattice/cpp/include
test_tetengo_lattice_LDFLAGS = \
    -L${top_builddir}/library/lattice/c/src
test_tetengo_lattice_LDADD = \
    -ltetengo.lattice \
    ${BOOST_UNIT_TEST_FRAMEWORK_LIB}
test_tetengo_lattice_SOURCES = ${sources}

TESTS = ${check_PROGRAMS}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_lattice_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
