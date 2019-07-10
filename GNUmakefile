.PHONY: all
all: test


BUILD_DIR=build
BUILD_DIR_TIMESTAMP=${BUILD_DIR}/timestamp

.PHONY: test
test: ${BUILD_DIR} ${BUILD_DIR_TIMESTAMP} install
	make -C ${BUILD_DIR} test

${BUILD_DIR_TIMESTAMP}: | ${BUILD_DIR}
${BUILD_DIR}:
	mkdir -p $@


.PHONY: install
install:
	cd ${BUILD_DIR}; cmake -DCMAKE_BUILD_TYPE=Debug ..; make


.PHONY: realclean
realclean: ${BUILD_DIR}
	rm -fr $<


DOCS_CONF_DIR=docs
DOCS_CONF=${DOCS_CONF_DIR}/Doxyfile.in

.PHONY: init
init: ${DOCS_CONF_DIR} ${DOCS_CONF}

${DOCS_CONF}: | ${DOCS_CONF_DIR}
${DOCS_CONF_DIR}:
	mkdir -p $@
${DOCS_CONF}:
	doxygen -g $@
