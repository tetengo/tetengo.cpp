# Automake Settings
# Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/

pkg_headers = \
    json/element.h \
    json/jsonParser.h \
    json/reader.h

extra_headers =

nobase_pkginclude_HEADERS = ${pkg_headers}

EXTRA_DIST = ${extra_headers}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}
iwyu_CPPFLAGS = \
    -I${top_srcdir}/library/json/c/include \
    -I${top_srcdir}/library/json/cpp/include

iwyu: ${addsuffix .iwyuout, ${pkg_headers} ${extra_headers}}

%.iwyuout: %
	mkdir -p ${dir $@}
	${IWYU} ${IWYU_OPTS_C} ${CPPFLAGS} ${iwyu_CPPFLAGS} ${CFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


format: ${addsuffix .formatout, ${pkg_headers} ${extra_headers}}

%.formatout: %
	CLANGFORMAT=${CLANGFORMAT} DOS2UNIX=${DOS2UNIX} ${top_srcdir}/kogyan/tool/call_clang-format.sh $< || true
	${MKDIR_P} ${dir $@}
	touch $@

.PHONY: clean-format
clean-format:
	-find -name "*.formatout" | xargs rm -f

clean-local: clean-format
