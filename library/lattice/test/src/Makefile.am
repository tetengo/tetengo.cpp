# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    master.cpp \
    test_tetengo.lattice.entry.cpp \
    test_tetengo.lattice.string_view.cpp \
    test_tetengo.lattice.unordered_map_vocabulary.cpp \
    test_tetengo.lattice.vocabulary.cpp

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


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${test_tetengo_lattice_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

clean-local:
	-find -name "*.iwyuout" | xargs rm -f


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
