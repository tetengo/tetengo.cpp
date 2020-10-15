# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers = \
    tetengo_lattice_constraint.hpp \
    tetengo_lattice_constraintElement.hpp \
    tetengo_lattice_lattice.hpp \
    tetengo_lattice_vocabulary.hpp

sources = \
    tetengo_lattice_constraint.cpp \
    tetengo_lattice_constraintElement.cpp \
    tetengo_lattice_entry.cpp \
    tetengo_lattice_lattice.cpp \
    tetengo_lattice_nBestIterator.cpp \
    tetengo_lattice_node.cpp \
    tetengo_lattice_path.cpp \
    tetengo_lattice_vocabulary.cpp

lib_LTLIBRARIES = libtetengo.lattice.la

libtetengo_lattice_la_CPPFLAGS = \
    -I${top_srcdir}/library/lattice/c/include \
    -I${top_srcdir}/library/lattice/cpp/include
libtetengo_lattice_la_LIBADD = \
    ${top_builddir}/library/lattice/cpp/src/libtetengo.lattice.noinst.la
libtetengo_lattice_la_SOURCES = ${headers} ${sources}

EXTRA_DIST = \
    dll_exports.def


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_lattice_la_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
