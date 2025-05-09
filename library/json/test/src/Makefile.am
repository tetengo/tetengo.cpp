# Automake Settings
# Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/

headers = \
    usage_tetengo.json.parsing_c.h \
    usage_tetengo.json.parsing_cpp.hpp

sources = \
    master.cpp \
    test_tetengo.json.channel.cpp \
    test_tetengo.json.comment_removing_reader.cpp \
    test_tetengo.json.element.cpp \
    test_tetengo.json.file_location.cpp \
    test_tetengo.json.json_grammar.cpp \
    test_tetengo.json.json_parser.cpp \
    test_tetengo.json.line_counting_reader.cpp \
    test_tetengo.json.reader.cpp \
    test_tetengo.json.reader_iterator.cpp \
    test_tetengo.json.stream_reader.cpp \
    usage_tetengo.json.cpp \
    usage_tetengo.json.parsing_c.c \
    usage_tetengo.json.parsing_cpp.cpp

check_PROGRAMS = test_tetengo.json

test_tetengo_json_CPPFLAGS = \
    -I${top_srcdir}/library/json/c/include \
    -I${top_srcdir}/library/json/cpp/include \
    -I${top_srcdir}/library/text/cpp/include
test_tetengo_json_LDFLAGS = \
    -L${top_builddir}/library/json/c/src \
    -L${top_builddir}/library/text/cpp/src
test_tetengo_json_LDADD = \
    -ltetengo.json \
    -ltetengo.text.cpp \
    ${BOOST_UNIT_TEST_FRAMEWORK_LIB}
test_tetengo_json_DEPENDENCIES = \
    ${top_builddir}/library/json/c/src/libtetengo.json.la \
    ${top_builddir}/library/text/cpp/src/libtetengo.text.noinst.la
test_tetengo_json_SOURCES = ${headers} ${sources}

TESTS = ${check_PROGRAMS}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.c.iwyuout: %.c
	${IWYU} ${IWYU_OPTS_C} ${CPPFLAGS} ${test_tetengo_json_CPPFLAGS} ${CFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.h.iwyuout: %.h
	${IWYU} ${IWYU_OPTS_C} ${CPPFLAGS} ${test_tetengo_json_CPPFLAGS} ${CFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.cpp.iwyuout: %.cpp
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_json_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.hpp.iwyuout: %.hpp
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_json_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
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
