# Automake Settings
# Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo_text_encoder.cpp \
    tetengo_text_graphemeSplitter.cpp

lib_LTLIBRARIES = libtetengo.text.la

libtetengo_text_la_CPPFLAGS = \
    -I${top_srcdir}/library/text/c/include \
    -I${top_srcdir}/library/text/cpp/include
libtetengo_text_la_LIBADD = \
    ${top_builddir}/library/text/cpp/src/libtetengo.text.noinst.la \
    ${top_builddir}/library/platform_dependent/cpp/src/unixos/libtetengo.platform_dependent.noinst.la
libtetengo_text_la_DEPENDENCIES = \
    ${top_builddir}/library/text/cpp/src/libtetengo.text.noinst.la \
    ${top_builddir}/library/platform_dependent/cpp/src/unixos/libtetengo.platform_dependent.noinst.la
libtetengo_text_la_SOURCES = ${headers} ${sources}

EXTRA_DIST = \
    dll_exports.def


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_text_la_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


format: ${addsuffix .formatout, ${headers} ${sources}}

%.formatout: %
	${top_srcdir}/kogyan/tool/call_clang-format.sh $< || true
	${MKDIR_P} ${dir $@}
	touch $@

.PHONY: clean-format
clean-format:
	-find -name "*.formatout" | xargs rm -f

clean-local: clean-format
