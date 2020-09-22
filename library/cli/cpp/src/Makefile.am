# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo.cli.character_width.cpp \
    tetengo.cli.terminal_string_width.cpp

lib_LIBRARIES = libtetengo.cli.cpp.a

libtetengo_cli_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/library/cli/cpp/include
libtetengo_cli_cpp_a_SOURCES = ${sources}

noinst_LTLIBRARIES = libtetengo.cli.noinst.la

libtetengo_cli_noinst_la_CPPFLAGS = ${libtetengo_cli_cpp_a_CPPFLAGS}
libtetengo_cli_noinst_la_CXXFLAGS = -fPIC
libtetengo_cli_noinst_la_LDFLAGS = -static
libtetengo_cli_noinst_la_SOURCES = ${libtetengo_cli_cpp_a_SOURCES}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${libtetengo_cli_cpp_a_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
