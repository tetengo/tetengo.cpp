# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers = \
    tetengo_json_element.hpp \
    tetengo_json_reader.hpp

sources = \
    tetengo_json_element.cpp \
    tetengo_json_jsonParser.cpp \
    tetengo_json_reader.cpp

lib_LTLIBRARIES = libtetengo.json.la

libtetengo_json_la_CPPFLAGS = \
    -I${top_srcdir}/library/json/c/include \
    -I${top_srcdir}/library/json/cpp/include
libtetengo_json_la_LIBADD = \
    ${top_builddir}/library/json/cpp/src/libtetengo.json.noinst.la
libtetengo_json_la_DEPENDENCIES = \
    ${top_builddir}/library/json/cpp/src/libtetengo.json.noinst.la
libtetengo_json_la_SOURCES = ${headers} ${sources}

EXTRA_DIST = \
    dll_exports.def


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_json_la_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
