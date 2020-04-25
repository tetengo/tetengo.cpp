# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo.lattice.entry.cpp \
    tetengo.lattice.lattice.cpp \
    tetengo.lattice.node.cpp \
    tetengo.lattice.unordered_map_vocabulary.cpp \
    tetengo.lattice.vocabulary.cpp

lib_LIBRARIES = libtetengo.lattice.cpp.a

libtetengo_lattice_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/library/lattice/cpp/include
libtetengo_lattice_cpp_a_SOURCES = ${sources}

noinst_LTLIBRARIES = libtetengo.lattice.noinst.la

libtetengo_lattice_noinst_la_CPPFLAGS = ${libtetengo_lattice_cpp_a_CPPFLAGS}
libtetengo_lattice_noinst_la_CXXFLAGS = -fPIC
libtetengo_lattice_noinst_la_LDFLAGS = -static
libtetengo_lattice_noinst_la_SOURCES = ${libtetengo_lattice_cpp_a_SOURCES}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${libtetengo_lattice_cpp_a_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
