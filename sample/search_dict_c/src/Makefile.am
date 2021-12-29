# Automake Settings
# Copyright (C) 2019-2022 kaoru  https://www.tetengo.org/

headers =

sources = \
    main.c

bin_PROGRAMS = search_dict_c

search_dict_c_CPPFLAGS = \
    -I${top_srcdir}/library/text/c/include \
    -I${top_srcdir}/library/trie/c/include
search_dict_c_LDFLAGS = \
    -L${top_builddir}/library/text/c/src \
    -L${top_builddir}/library/trie/c/src
search_dict_c_LDADD = \
    -ltetengo.text \
    -ltetengo.trie
search_dict_c_DEPENDENCIES = \
    ${top_builddir}/library/text/c/src/libtetengo.text.la \
    ${top_builddir}/library/trie/c/src/libtetengo.trie.la
search_dict_c_SOURCES = ${sources}


IWYU_OPTS_C += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_C} ${CPPFLAGS} ${search_dict_c_CPPFLAGS} ${CFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
