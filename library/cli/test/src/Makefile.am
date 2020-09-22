# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    master.cpp \
    test_tetengo.cli.terminal_string_width.cpp

check_PROGRAMS = test_tetengo.cli

test_tetengo_cli_CPPFLAGS = \
    -I${top_srcdir}/library/cli/c/include \
    -I${top_srcdir}/library/cli/cpp/include
test_tetengo_cli_LDFLAGS = \
    -L${top_builddir}/library/cli/c/src
test_tetengo_cli_LDADD = \
    -ltetengo.cli \
    ${BOOST_UNIT_TEST_FRAMEWORK_LIB}
test_tetengo_cli_SOURCES = ${sources}

TESTS = ${check_PROGRAMS}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${test_tetengo_cli_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
