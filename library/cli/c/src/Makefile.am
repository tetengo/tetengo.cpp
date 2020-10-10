# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo_cli_graphemeSplitter.cpp

lib_LTLIBRARIES = libtetengo.cli.la

libtetengo_cli_la_CPPFLAGS = \
    -I${top_srcdir}/library/cli/c/include \
    -I${top_srcdir}/library/cli/cpp/include
libtetengo_cli_la_LIBADD = \
    ${top_builddir}/library/cli/cpp/src/libtetengo.cli.noinst.la
libtetengo_cli_la_SOURCES = ${headers} ${sources}

EXTRA_DIST = \
    dll_exports.def


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${libtetengo_cli_la_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


format: ${addsuffix .formatout, ${headers} ${sources}}

%.formatout: %
	${top_srcdir}/tools/call_clang-format.sh $< || true
	touch $@

.PHONY: clean-format
clean-format:
	-find -name "*.formatout" | xargs rm -f

clean-local: clean-format
