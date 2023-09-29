MAKE := make

CC := gcc
CFLAGS := -Wall

SOURCEDIR := .
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')
MAIN_OUT := cysprog.out

.PHONY: hello
hello:
ifeq (${f},)
	@echo "f={file} must be provided"
else
# get base name
	$(eval base := $(lastword $(subst /, ,$(dir $(abspath ${f:c=out})))))
# create ./bin/${base} if dir doesn't exit
	@if [ ! -d "./bin/${base}" ]; then mkdir "./bin/${base}"; fi;
# compile single c file
	@${CC} ${CFLAGS} ${f} -o "./bin/${base}/$(notdir ${f:c=out})"
	@echo "./bin/${base}/$(notdir ${f:c=out})"
endif

.PHONY: lab1_notmain
lab1_notmain:
	${MAKE} -C ./lab1/syscall/ notmain

# all:
# 	${CC} ${CFLAGS} SOURCES -o ./bin/${MAIN_OUT}
# 	./bin/${MAIN_OUT}
