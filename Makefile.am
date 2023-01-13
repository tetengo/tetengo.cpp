# Automake Settings
# Copyright (C) 2019-2023 kaoru  https://www.tetengo.org/

ACLOCAL_AMFLAGS = -I m4

SOURCE_SUBDIRS = \
    executable \
    library \
    sample

SCRIPT_SUBDIRS = \
    kogyan \
    setup \
    tool

SUBDIRS = \
    ${SOURCE_SUBDIRS} \
    ${SCRIPT_SUBDIRS} \
    docsrc \
    precompiled

data_files = \
    COPYING \
    README.md

EXTRA_DIST = \
    ${data_files} \
    bootstrap.sh \
    Doxyfile \
    tetengo.sln \
    tetengo.setup.sln


nobase_pkgdata_DATA = \
    ${data_files}


.PHONY: precompiled_h
precompiled_h:
	cd ${top_srcdir} && ./kogyan/tool/make_precompiled_h.py

.PHONY: doc
doc:
	cd ${top_srcdir} && ${DOXYGEN} Doxyfile
	if test "${top_srcdir}" != "${top_builddir}"; \
	then \
	    rm -rf ${top_builddir}/doc; \
	    mv ${top_srcdir}/doc ${top_builddir}; \
	fi

.PHONY: clean-doc
clean-doc:
	-rm -rf ${top_builddir}/doc

distclean-local: clean-doc

iwyu: ${SOURCE_SUBDIRS}
	IWYU_EXIT_CODE="0"; \
	for f in $$(find -name "*.iwyuout"); \
	do \
	    ${top_srcdir}/kogyan/tool/filter_iwyuout.py < $$f; \
	    IWYU_EXIT_CODE_ITER=$$?; \
	    test "x$$IWYU_EXIT_CODE" == "x0" && IWYU_EXIT_CODE=$$IWYU_EXIT_CODE_ITER; \
	done; \
	exit $$IWYU_EXIT_CODE

clean-iwyu: ${SOURCE_SUBDIRS}

.PHONY: check_file_name_lengths
check_file_name_lengths:
	cd ${top_srcdir} && ./kogyan/tool/check_file_name_lengths.py

mypy: ${SCRIPT_SUBDIRS}

${top_srcdir}/.clang-format: kogyan/.clang-format
	cp $< $@

format: ${top_srcdir}/.clang-format ${SOURCE_SUBDIRS} ${SCRIPT_SUBDIRS}

clean-format: ${SOURCE_SUBDIRS}
	-rm -f .clang-format

.PHONY: ${SOURCE_SUBDIRS}
${SOURCE_SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}

.PHONY: ${SCRIPT_SUBDIRS}
${SCRIPT_SUBDIRS}:
	${MAKE} -C $@ ${MAKECMDGOALS}

.PHONY: pushcheck
pushcheck:
	${MAKE} -s check_file_name_lengths
	${MAKE} -s format
	${MAKE} check
	${MAKE} iwyu
	${MAKE} mypy
	${MAKE} doc > /dev/null
