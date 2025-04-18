# Automake Settings
# Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/

headers = \
    tetengo_lattice_constraint.hpp \
    tetengo_lattice_constraintElement.hpp \
    tetengo_lattice_input.hpp \
    tetengo_lattice_lattice.hpp \
    tetengo_lattice_vocabulary.hpp

sources = \
    tetengo_lattice_constraint.cpp \
    tetengo_lattice_constraintElement.cpp \
    tetengo_lattice_entry.cpp \
    tetengo_lattice_input.cpp \
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
libtetengo_lattice_la_DEPENDENCIES = \
    ${top_builddir}/library/lattice/cpp/src/libtetengo.lattice.noinst.la
libtetengo_lattice_la_SOURCES = ${headers} ${sources}

EXTRA_DIST = \
    dll_exports.def


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_lattice_la_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
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
