# Automake Settings
# Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

headers = \
    usage_tetengo.text.encoding_c.h \
    usage_tetengo.text.encoding_cpp.hpp \
    usage_tetengo.text.graphemeSplit_c.h \
    usage_tetengo.text.grapheme_split_cpp.hpp

sources = \
    master.cpp \
    test_tetengo.text.encoder.cpp \
    test_tetengo.text.encoding.cp932.cpp \
    test_tetengo.text.encoding.utf16.cpp \
    test_tetengo.text.encoding.utf8.cpp \
    test_tetengo.text.grapheme_splitter.cpp \
    test_tetengo.text.grapheme_splitting.character_width.cpp \
    test_tetengo.text.grapheme_splitting.default_charactX.cpp \
    test_tetengo.text.grapheme_splitting.east_asian_charX.cpp \
    test_tetengo.text.grapheme_splitting.grapheme_segment.cpp \
    usage_tetengo.text.cpp \
    usage_tetengo.text.encoding_c.c \
    usage_tetengo.text.encoding_cpp.cpp \
    usage_tetengo.text.graphemeSplit_c.c \
    usage_tetengo.text.grapheme_split_cpp.cpp

check_PROGRAMS = test_tetengo.text

test_tetengo_text_CPPFLAGS = \
    -I${top_srcdir}/library/text/c/include \
    -I${top_srcdir}/library/text/cpp/include
test_tetengo_text_LDFLAGS = \
    -L${top_builddir}/library/text/c/src
test_tetengo_text_LDADD = \
    -ltetengo.text \
    ${BOOST_UNIT_TEST_FRAMEWORK_LIB}
test_tetengo_text_DEPENDENCIES = \
    ${top_builddir}/library/text/c/src/libtetengo.text.la
test_tetengo_text_SOURCES = ${headers} ${sources}

TESTS = ${check_PROGRAMS}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_text_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
