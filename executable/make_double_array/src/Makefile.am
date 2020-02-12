# Automake Settings
# Copyright (C) 2019 kaoru

headers =

sources = \
    main.cpp

bin_PROGRAMS = make_double_array

make_double_array_CPPFLAGS = \
    -I${top_srcdir}/library/trie/cpp/include
make_double_array_LDFLAGS = \
    -L${top_builddir}/library/trie/cpp/src
make_double_array_LDADD = \
    -ltetengo.trie.cpp
make_double_array_SOURCES = ${sources}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${make_double_array_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

clean-local:
	-find -name "*.iwyuout" | xargs rm -f


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
