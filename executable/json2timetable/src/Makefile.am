# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

headers = \
    arrayList.h \
    timetable.h

sources = \
    arrayList.c \
    main.c \
    timetable.c

bin_PROGRAMS = json2timetable

json2timetable_CPPFLAGS = \
    -I${top_srcdir}/library/json/c/include
json2timetable_LDFLAGS = \
    -L${top_builddir}/library/json/c/src
json2timetable_LDADD = \
    -ltetengo.json
json2timetable_SOURCES = ${headers} ${sources}


IWYU_OPTS_C += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_C} ${CPPFLAGS} ${json2timetable_CPPFLAGS} ${CFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
