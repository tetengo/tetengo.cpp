# Automake Settings
# Copyright (C) 2019-2025 kaoru  https://www.tetengo.org/

headers =

sources = \
    tetengo.platform_dependent.propX.cpp \
    tetengo.platform_dependent.textX.cpp \
    tetengo.platform_dependent.windX.cpp

EXTRA_DIST = \
    ${headers} ${sources}

libtetengo_platform_dependent_cpp_a_CPPFLAGS = \
    -I${top_srcdir}/library/platform_dependent/cpp/include

IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}

iwyu: ${addsuffix .iwyuout, ${headers} ${sources}}

%.iwyuout: %
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${libtetengo_platform_dependent_cpp_a_CPPFLAGS} ${CXXFLAGS_IWYU} $< 2> ${addsuffix .tmp, $@} || true
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
