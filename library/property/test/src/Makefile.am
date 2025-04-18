# Automake Settings
# Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/

headers = \
    usage_tetengo.property.save_load_cpp.hpp \
    usage_tetengo.property.saveLoad_c.h

sources = \
    master.cpp \
    test_tetengo.property.file_storage.cpp \
    test_tetengo.property.memory_storage.cpp \
    test_tetengo.property.property_set.cpp \
    test_tetengo.property.storage.cpp \
    test_tetengo.property.storage_proxy.cpp \
    test_tetengo.property.windows_registry_storage.cpp \
    usage_tetengo.property.cpp \
    usage_tetengo.property.save_load_cpp.cpp \
    usage_tetengo.property.saveLoad_c.c

check_PROGRAMS = test_tetengo.property

test_tetengo_property_CPPFLAGS = \
    -I${top_srcdir}/library/platform_dependent/cpp/include \
    -I${top_srcdir}/library/property/c/include \
    -I${top_srcdir}/library/property/cpp/include
test_tetengo_property_LDFLAGS = \
    -L${top_builddir}/library/json/cpp/src \
    -L${top_builddir}/library/platform_dependent/cpp/src/unixos \
    -L${top_builddir}/library/property/c/src \
    -L${top_builddir}/library/text/cpp/src
test_tetengo_property_LDADD = \
    -ltetengo.property \
    -ltetengo.json.cpp \
    -ltetengo.text.cpp \
    -ltetengo.platform_dependent.cpp \
    ${BOOST_UNIT_TEST_FRAMEWORK_LIB}
test_tetengo_property_DEPENDENCIES = \
    ${top_builddir}/library/json/cpp/src/libtetengo.json.noinst.la \
    ${top_builddir}/library/platform_dependent/cpp/src/unixos/libtetengo.platform_dependent.noinst.la \
    ${top_builddir}/library/property/c/src/libtetengo.property.la \
    ${top_builddir}/library/text/cpp/src/libtetengo.text.noinst.la
test_tetengo_property_SOURCES = ${headers} ${sources}

TESTS = ${check_PROGRAMS}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.c.iwyuout: %.c
	${IWYU} ${IWYU_OPTS_C} ${CPPFLAGS} ${test_tetengo_property_CPPFLAGS} ${CFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.h.iwyuout: %.h
	${IWYU} ${IWYU_OPTS_C} ${CPPFLAGS} ${test_tetengo_property_CPPFLAGS} ${CFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.cpp.iwyuout: %.cpp
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_property_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

%.hpp.iwyuout: %.hpp
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${test_tetengo_property_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
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
