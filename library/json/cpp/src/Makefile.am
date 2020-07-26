# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo.json.json_parser.cpp

lib_LIBRARIES = libtetengo.json.cpp.a

libtetengo_json_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/library/json/cpp/include
libtetengo_json_cpp_a_SOURCES = ${sources}

noinst_LTLIBRARIES = libtetengo.json.noinst.la

libtetengo_json_noinst_la_CPPFLAGS = ${libtetengo_json_cpp_a_CPPFLAGS}
libtetengo_json_noinst_la_CXXFLAGS = -fPIC
libtetengo_json_noinst_la_LDFLAGS = -static
libtetengo_json_noinst_la_SOURCES = ${libtetengo_json_cpp_a_SOURCES}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${libtetengo_json_cpp_a_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
