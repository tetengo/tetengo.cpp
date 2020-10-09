# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

pkg_headers = \
    trie/storage.h \
    trie/trie.h \
    trie/trieIterator.h

extra_headers =

nobase_pkginclude_HEADERS = ${pkg_headers}

EXTRA_DIST = ${extra_headers}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}
iwyu_CPPFLAGS = \
    -I${top_srcdir}/library/trie/cpp/include

iwyu: ${addsuffix .iwyuout, ${pkg_headers} ${extra_headers}}

%.iwyuout: %
	mkdir -p ${dir $@}
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${iwyu_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


format: ${addsuffix .formatout, ${pkg_headers} ${extra_headers}}

%.formatout: %
	${top_srcdir}/tools/call_clang-format.sh $< || true
	touch $@

.PHONY: clean-format
clean-format:
	-find -name "*.formatout" | xargs rm -f

clean-local: clean-format
