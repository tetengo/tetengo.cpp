# Automake Settings
# Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

headers = \
    tetengo_property_storage.hpp

sources = \
    tetengo_property_propertySet.cpp \
    tetengo_property_storage.cpp

lib_LTLIBRARIES = libtetengo.property.la

libtetengo_property_la_CPPFLAGS = \
    -I${top_srcdir}/library/property/c/include \
    -I${top_srcdir}/library/property/cpp/include
libtetengo_property_la_LIBADD = \
    ${top_builddir}/library/property/cpp/src/libtetengo.property.noinst.la
libtetengo_property_la_DEPENDENCIES = \
    ${top_builddir}/library/property/cpp/src/libtetengo.property.noinst.la
libtetengo_property_la_SOURCES = ${headers} ${sources}

EXTRA_DIST = \
    dll_exports.def


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_property_la_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
