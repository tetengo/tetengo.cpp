# Automake Settings
# Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/

pkg_headers = \
    text/encoder.hpp \
    text/encoding/cp932.hpp \
    text/encoding/utf16.hpp \
    text/encoding/utf8.hpp \
    text/grapheme_splitter.hpp \
    text/grapheme_splitting/character_width.hpp \
    text/grapheme_splitting/default_characterX.hpp \
    text/grapheme_splitting/east_asian_characX.hpp \
    text/grapheme_splitting/grapheme_segment.hpp

extra_headers = \
    text/0namespace.dox \
    text/grapheme_splitting/0namespace.dox

nobase_pkginclude_HEADERS = ${pkg_headers}

EXTRA_DIST = ${extra_headers}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}
iwyu_CPPFLAGS = \
    -I${top_srcdir}/library/text/cpp/include

iwyu: ${addsuffix .iwyuout, ${pkg_headers} ${extra_headers}}

%.iwyuout: %
	mkdir -p ${dir $@}
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${iwyu_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


format: ${addsuffix .formatout, ${pkg_headers} ${extra_headers}}

%.formatout: %
	CLANGFORMAT=${CLANGFORMAT} DOS2UNIX=${DOS2UNIX} ${top_srcdir}/kogyan/tool/call_clang-format.sh $< || true
	${MKDIR_P} ${dir $@}
	touch $@

.PHONY: clean-format
clean-format:
	-find -name "*.formatout" | xargs rm -f

clean-local: clean-format
