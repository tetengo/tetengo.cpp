# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers = \
    timetable_vocabulary.hpp

sources = \
    main.cpp \
    timetable_vocabulary.cpp

bin_PROGRAMS = transfer_trains

transfer_trains_CPPFLAGS =
transfer_trains_LDFLAGS =
transfer_trains_LDADD =
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


.PHONY: format
format:
	for f in ${headers} ${sources}; do ${top_srcdir}/tools/call_clang-format.sh ${srcdir}/$$f; done
