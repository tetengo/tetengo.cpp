# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers =

sources = \
    main.cpp

bin_PROGRAMS = search_dict

search_dict_CPPFLAGS = \
    -I${top_srcdir}/library/text/cpp/include \
    -I${top_srcdir}/library/trie/cpp/include
search_dict_LDFLAGS = \
    -L${top_builddir}/library/platform_dependent/cpp/src/unixos \
    -L${top_builddir}/library/text/cpp/src \
    -L${top_builddir}/library/trie/cpp/src
search_dict_LDADD = \
    -ltetengo.text.cpp \
    -ltetengo.platform_dependent.cpp \
    -ltetengo.trie.cpp
search_dict_DEPENDENCIES = \
    ${top_builddir}/library/platform_dependent/cpp/src/unixos/libtetengo.platform_dependent.noinst.la \
    ${top_builddir}/library/text/cpp/src/libtetengo.text.noinst.la \
    ${top_builddir}/library/trie/cpp/src/libtetengo.trie.noinst.la
search_dict_SOURCES = ${sources}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${search_dict_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
