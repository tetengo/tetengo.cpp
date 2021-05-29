# Automake Settings
# Copyright (C) 2019-2021 kaoru  https://www.tetengo.org/

headers = \
    tetengo.property.json_parser.hpp \
    tetengo.property.json_writer.hpp

sources = \
    tetengo.property.file_storage.cpp \
    tetengo.property.json_parser.cpp \
    tetengo.property.json_writer.cpp \
    tetengo.property.memory_storage.cpp \
    tetengo.property.property_set.cpp \
    tetengo.property.storage.cpp \
    tetengo.property.windows_registry_storage.cpp

lib_LIBRARIES = libtetengo.property.cpp.a

libtetengo_property_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/library/json/cpp/include \
    -I${top_srcdir}/library/platform_dependent/cpp/include \
    -I${top_srcdir}/library/property/cpp/include \
    -I${top_srcdir}/library/text/cpp/include
libtetengo_property_cpp_a_SOURCES = ${headers} ${sources}

noinst_LTLIBRARIES = libtetengo.property.noinst.la

libtetengo_property_noinst_la_CPPFLAGS = ${libtetengo_property_cpp_a_CPPFLAGS}
libtetengo_property_noinst_la_CXXFLAGS = -fPIC
libtetengo_property_noinst_la_LDFLAGS = -static
libtetengo_property_noinst_la_SOURCES = ${libtetengo_property_cpp_a_SOURCES}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_property_cpp_a_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
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
