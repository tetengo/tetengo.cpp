# Automake Settings
# $12026 kaoru  https://www.tetengo.org/

pkg_headers = \
    json/channel.hpp \
    json/comment_removing_reader.hpp \
    json/element.hpp \
    json/file_location.hpp \
    json/json_grammar.hpp \
    json/json_parser.hpp \
    json/line_counting_reader.hpp \
    json/reader.hpp \
    json/reader_iterator.hpp \
    json/stream_reader.hpp

extra_headers = \
    json/0namespace.dox

nobase_pkginclude_HEADERS = ${pkg_headers}

EXTRA_DIST = ${extra_headers}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}
iwyu_CPPFLAGS = \
    -I${top_srcdir}/library/json/cpp/include

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
