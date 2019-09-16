# Automake Settings
# Copyright (C) 2019 kaoru

pkg_headers = \
    trie/double_array.hpp

extra_headers = \
    trie/0namespace.dox

nobase_pkginclude_HEADERS = ${pkg_headers}

EXTRA_DIST = ${extra_headers}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}
iwyu_CPPFLAGS = \
    -I${top_srcdir}/trie/cpp/include

iwyu: ${addsuffix .iwyuout, ${pkg_headers} ${extra_headers}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${iwyu_CPPFLAGS} ${CXXFLAGS} $< 2> ${srcdir}/${addsuffix .tmp, $@} || true
	mv -f ${srcdir}/${addsuffix .tmp, $@} ${srcdir}/$@

clean-local:
	-find ${srcdir} -name *.iwyuout | xargs rm -f


.PHONY: format
format:
	for f in ${pkg_headers} ${extra_headers}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
