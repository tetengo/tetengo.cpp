# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    master.cpp \
    test_tetengo.json.channel.cpp \
    test_tetengo.json.comment_removing_reader.cpp \
    test_tetengo.json.element.cpp \
    test_tetengo.json.json_grammar.cpp \
    test_tetengo.json.json_parser.cpp \
    test_tetengo.json.line_counting_reader.cpp \
    test_tetengo.json.reader.cpp \
    test_tetengo.json.reader_iterator.cpp \
    test_tetengo.json.stream_reader.cpp

check_PROGRAMS = test_tetengo.json

test_tetengo_json_CPPFLAGS = \
    -I${top_srcdir}/library/json/c/include \
    -I${top_srcdir}/library/json/cpp/include
test_tetengo_json_LDFLAGS = \
    -L${top_builddir}/library/json/c/src
test_tetengo_json_LDADD = \
    -ltetengo.json \
    ${BOOST_UNIT_TEST_FRAMEWORK_LIB}
test_tetengo_json_SOURCES = ${sources}

TESTS = ${check_PROGRAMS}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${test_tetengo_json_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
