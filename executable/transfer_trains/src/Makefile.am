# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers = \
    timetable.hpp

sources = \
    main.cpp \
    timetable.cpp

bin_PROGRAMS = transfer_trains

transfer_trains_CPPFLAGS = \
    -I${top_srcdir}/library/lattice/cpp/include
transfer_trains_LDFLAGS = \
    -L${top_builddir}/library/lattice/cpp/src
transfer_trains_LDADD = \
    -ltetengo.lattice.cpp
transfer_trains_SOURCES = ${headers} ${sources}


IWYU_OPTS += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS} ${CPPFLAGS} ${transfer_trains_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
