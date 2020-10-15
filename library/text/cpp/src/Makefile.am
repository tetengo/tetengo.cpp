# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers = \
    tetengo.text.character_property_map.hpp

sources = \
    tetengo.text.character_property_map.cpp \
    tetengo.text.character_width.cpp \
    tetengo.text.default_character_width.cpp \
    tetengo.text.east_asian_character_width.cpp \
    tetengo.text.grapheme_segment.cpp \
    tetengo.text.grapheme_splitter.cpp

lib_LIBRARIES = libtetengo.text.cpp.a

libtetengo_text_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/library/text/cpp/include
libtetengo_text_cpp_a_SOURCES = ${headers} ${sources}

noinst_LTLIBRARIES = libtetengo.text.noinst.la

libtetengo_text_noinst_la_CPPFLAGS = ${libtetengo_text_cpp_a_CPPFLAGS}
libtetengo_text_noinst_la_CXXFLAGS = -fPIC
libtetengo_text_noinst_la_LDFLAGS = -static
libtetengo_text_noinst_la_SOURCES = ${libtetengo_text_cpp_a_SOURCES}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_text_cpp_a_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
