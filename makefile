export ROOT = $(PWD)


.PHONY: all
all : build-y86lib build-y86run

include makefile.common


.PHONY: build-y86lib
build-y86lib: | $(OBJ_DIR)
	$(info >>>>)
	make -C $(OBJ_DIR) -f $(ROOT)/y86lib.mk

.PHONY: build-y86run
build-y86run: build-y86lib | $(OBJ_DIR)
	$(info >>>>)
	make -C $(OBJ_DIR) -f $(ROOT)/y86run.mk
