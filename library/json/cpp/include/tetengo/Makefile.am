# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

pkg_headers = \
    json/comment_removing_reader.hpp \
    json/json_parser.hpp \
    json/reader.hpp \
    json/reader_iterator.hpp \
    json/stream_reader.hpp

extra_headers = \
    json/0namespace.dox

nobase_pkginclude_HEADERS = ${pkg_headers}

EXTRA_DIST = ${extra_headers}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}
iwyu_CPPFLAGS = \
    -I${top_srcdir}/library/json/cpp/include

iwyu: ${addsuffix .iwyuout, ${pkg_headers} ${extra_headers}}

%.iwyuout: %
	mkdir -p ${dir $@}
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${iwyu_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


.PHONY: format
format:
	for f in ${pkg_headers} ${extra_headers}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
