# Automake Settings
# Copyright (C) 2019 kaoru

pkg_headers = \
    trie/default_key_serializer.hpp \
    trie/double_array.hpp \
    trie/enumerator.hpp \
    trie/memory_storage.hpp \
    trie/shared_storage.hpp \
    trie/storage.hpp \
    trie/trie.hpp

extra_headers = \
    trie/0namespace.dox

nobase_pkginclude_HEADERS = ${pkg_headers}

EXTRA_DIST = ${extra_headers}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}
iwyu_CPPFLAGS = \
    -I${top_srcdir}/trie/cpp/include

iwyu: ${addsuffix .iwyuout, ${pkg_headers} ${extra_headers}}

%.iwyuout: %
	mkdir -p ${dir $@}
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${iwyu_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

clean-local:
	-find -name "*.iwyuout" | xargs rm -f


.PHONY: format
format:
	for f in ${pkg_headers} ${extra_headers}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
