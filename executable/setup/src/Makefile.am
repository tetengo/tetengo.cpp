# Automake Settings
# Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/

headers =

sources = \
    winmain.cpp

EXTRA_DIST = \
    ${headers} \
    ${sources}


iwyu:

.PHONY: clean-iwyu
clean-iwyu:

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
