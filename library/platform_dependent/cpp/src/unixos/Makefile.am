# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo.platform_dependent.text_encoder.cpp

lib_LIBRARIES = libtetengo.platform_dependent.cpp.a

libtetengo_platform_dependent_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/library/platform_dependent/cpp/include
libtetengo_platform_dependent_cpp_a_SOURCES = ${headers} ${sources}

noinst_LTLIBRARIES = libtetengo.platform_dependent.noinst.la

libtetengo_platform_dependent_noinst_la_CPPFLAGS = ${libtetengo_platform_dependent_cpp_a_CPPFLAGS}
libtetengo_platform_dependent_noinst_la_CXXFLAGS = -fPIC
libtetengo_platform_dependent_noinst_la_LDFLAGS = -static
libtetengo_platform_dependent_noinst_la_SOURCES = ${libtetengo_platform_dependent_cpp_a_SOURCES}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_platform_dependent_cpp_a_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
