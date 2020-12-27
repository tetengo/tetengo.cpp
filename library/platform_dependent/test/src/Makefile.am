# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    master.cpp \
    test_tetengo.platform_dependent.text_eX.cpp

check_PROGRAMS = test_tetengo.platform_dependent

test_tetengo_platform_dependent_CPPFLAGS = \
    -I${top_srcdir}/library/platform_dependent/cpp/include
test_tetengo_platform_dependent_LDFLAGS = \
    -L${top_builddir}/library/platform_dependent/cpp/src/unixos
test_tetengo_platform_dependent_LDADD = \
    -ltetengo.platform_dependent.cpp \
    ${BOOST_UNIT_TEST_FRAMEWORK_LIB}
test_tetengo_platform_dependent_DEPENDENCIES = \
    ${top_builddir}/library/platform_dependent/cpp/src/unixos/libtetengo.platform_dependent.noinst.la
test_tetengo_platform_dependent_SOURCES = ${sources}

TESTS = ${check_PROGRAMS}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_platform_dependent_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


format: ${addsuffix .formatout, ${headers} ${sources}}

%.formatout: %
	${top_srcdir}/tool/call_clang-format.sh $< || true
	${MKDIR_P} ${dir $@}
	touch $@

.PHONY: clean-format
clean-format:
	-find -name "*.formatout" | xargs rm -f

clean-local: clean-format
