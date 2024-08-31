# Automake Settings
# Copyright (C) 2019-2024 kaoru  https://www.tetengo.org/

headers = \
    timetable.hpp

sources = \
    main.cpp \
    timetable.cpp

bin_PROGRAMS = transfer_trains

transfer_trains_CPPFLAGS = \
    -I${top_srcdir}/library/lattice/cpp/include \
    -I${top_srcdir}/library/text/cpp/include
transfer_trains_LDFLAGS = \
    -L${top_builddir}/library/platform_dependent/cpp/src/unixos \
    -L${top_builddir}/library/lattice/cpp/src \
    -L${top_builddir}/library/text/cpp/src
transfer_trains_LDADD = \
    -ltetengo.lattice.cpp \
    -ltetengo.text.cpp \
    -ltetengo.platform_dependent.cpp
transfer_trains_DEPENDENCIES = \
    ${top_builddir}/library/platform_dependent/cpp/src/unixos/libtetengo.platform_dependent.noinst.la \
    ${top_builddir}/library/lattice/cpp/src/libtetengo.lattice.noinst.la \
    ${top_builddir}/library/text/cpp/src/libtetengo.text.noinst.la
transfer_trains_SOURCES = ${headers} ${sources}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${transfer_trains_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
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
