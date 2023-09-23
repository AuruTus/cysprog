CC := gcc
CFLAGS := -Wall

SOURCEDIR := .
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')
MAIN_OUT := cysprog.out

hello:
ifeq (${f},)
	@echo "f={file} must be provided"
else
	@${CC} ${CFLAGS} ${f} -o ./bin/${f:c=out}
endif

# all:
# 	${CC} ${CFLAGS} SOURCES -o ./bin/${MAIN_OUT}
# 	./bin/${MAIN_OUT}
