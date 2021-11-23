# Automake Settings
# Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo.lattice.constraint.cpp \
    tetengo.lattice.constraint_element.cpp \
    tetengo.lattice.entry.cpp \
    tetengo.lattice.lattice.cpp \
    tetengo.lattice.n_best_iterator.cpp \
    tetengo.lattice.node.cpp \
    tetengo.lattice.node_constraint_element.cpp \
    tetengo.lattice.path.cpp \
    tetengo.lattice.unordered_map_vocabulary.cpp \
    tetengo.lattice.vocabulary.cpp \
    tetengo.lattice.vocabulary_key.cpp \
    tetengo.lattice.wildcard_constraint_element.cpp

lib_LIBRARIES = libtetengo.lattice.cpp.a

libtetengo_lattice_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/library/lattice/cpp/include
libtetengo_lattice_cpp_a_SOURCES = ${headers} ${sources}

noinst_LTLIBRARIES = libtetengo.lattice.noinst.la

libtetengo_lattice_noinst_la_CPPFLAGS = ${libtetengo_lattice_cpp_a_CPPFLAGS}
libtetengo_lattice_noinst_la_CXXFLAGS = -fPIC
libtetengo_lattice_noinst_la_LDFLAGS = -static
libtetengo_lattice_noinst_la_SOURCES = ${libtetengo_lattice_cpp_a_SOURCES}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_lattice_cpp_a_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
