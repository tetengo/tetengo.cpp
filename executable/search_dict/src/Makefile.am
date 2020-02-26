# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    main.cpp

bin_PROGRAMS = search_dict

search_dict_CPPFLAGS = \
    -I${top_srcdir}/library/trie/cpp/include
search_dict_LDFLAGS = \
    -L${top_builddir}/library/trie/cpp/src
search_dict_LDADD = \
    -ltetengo.trie.cpp
search_dict_SOURCES = ${sources}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${search_dict_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

clean-local:
	-find -name "*.iwyuout" | xargs rm -f


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
