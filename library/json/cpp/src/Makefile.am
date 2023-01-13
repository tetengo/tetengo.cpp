# Automake Settings
# Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo.json.channel.cpp \
    tetengo.json.comment_removing_reader.cpp \
    tetengo.json.element.cpp \
    tetengo.json.file_location.cpp \
    tetengo.json.json_grammar.cpp \
    tetengo.json.json_parser.cpp \
    tetengo.json.line_counting_reader.cpp \
    tetengo.json.reader.cpp \
    tetengo.json.reader_iterator.cpp \
    tetengo.json.stream_reader.cpp

lib_LIBRARIES = libtetengo.json.cpp.a

libtetengo_json_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/library/json/cpp/include
libtetengo_json_cpp_a_SOURCES = ${headers} ${sources}

noinst_LTLIBRARIES = libtetengo.json.noinst.la

libtetengo_json_noinst_la_CPPFLAGS = ${libtetengo_json_cpp_a_CPPFLAGS}
libtetengo_json_noinst_la_CXXFLAGS = -fPIC
libtetengo_json_noinst_la_LDFLAGS = -static
libtetengo_json_noinst_la_SOURCES = ${libtetengo_json_cpp_a_SOURCES}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_json_cpp_a_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
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
