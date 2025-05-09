# Automake Settings
# Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/

headers = \
    arrayList.h \
    encode.h \
    loadTimetable.h \
    printLineTimetable.h \
    printStationTimetable.h \
    timetable.h

sources = \
    arrayList.c \
    encode.c \
    loadTimetable.c \
    main.c \
    printLineTimetable.c \
    printStationTimetable.c \
    timetable.c

bin_PROGRAMS = json2timetable

json2timetable_CPPFLAGS = \
    -I${top_srcdir}/library/json/c/include \
    -I${top_srcdir}/library/text/c/include
json2timetable_LDFLAGS = \
    -L${top_builddir}/library/json/c/src \
    -L${top_builddir}/library/text/c/src
json2timetable_LDADD = \
    -ltetengo.json \
    -ltetengo.text
json2timetable_DEPENDENCIES = \
    ${top_builddir}/library/json/c/src/libtetengo.json.la \
    ${top_builddir}/library/text/c/src/libtetengo.text.la
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
	CLANGFORMAT=${CLANGFORMAT} DOS2UNIX=${DOS2UNIX} ${top_srcdir}/kogyan/tool/call_clang-format.sh $< || true
	${MKDIR_P} ${dir $@}
	touch $@

.PHONY: clean-format
clean-format:
	-find -name "*.formatout" | xargs rm -f

clean-local: clean-format
