# Automake Settings
# Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/

pkg_headers = \
    lattice/connection.hpp \
    lattice/constraint.hpp \
    lattice/constraint_element.hpp \
    lattice/entry.hpp \
    lattice/lattice.hpp \
    lattice/n_best_iterator.hpp \
    lattice/node.hpp \
    lattice/node_constraint_element.hpp \
    lattice/path.hpp \
    lattice/unordered_map_vocabulary.hpp \
    lattice/vocabulary.hpp \
    lattice/wildcard_constraint_element.hpp

extra_headers = \
    lattice/0namespace.dox

nobase_pkginclude_HEADERS = ${pkg_headers}

EXTRA_DIST = ${extra_headers}


IWYU_OPTS_CXX += -Xiwyu --mapping_file=${top_srcdir}/${IWYU_IMP_PATH}
iwyu_CPPFLAGS = \
    -I${top_srcdir}/library/lattice/cpp/include

iwyu: ${addsuffix .iwyuout, ${pkg_headers} ${extra_headers}}

%.iwyuout: %
	mkdir -p ${dir $@}
	${IWYU} ${IWYU_OPTS_CXX} ${CPPFLAGS} ${iwyu_CPPFLAGS} ${CXXFLAGS} $< 2> ${addsuffix .tmp, $@} || true
	mv -f ${addsuffix .tmp, $@} $@

.PHONY: clean-iwyu
clean-iwyu:
	-find -name "*.iwyuout" | xargs rm -f

clean-local: clean-iwyu


format: ${addsuffix .formatout, ${pkg_headers} ${extra_headers}}

%.formatout: %
	${top_srcdir}/tool/call_clang-format.sh $< || true
	${MKDIR_P} ${dir $@}
	touch $@

.PHONY: clean-format
clean-format:
	-find -name "*.formatout" | xargs rm -f

clean-local: clean-format
