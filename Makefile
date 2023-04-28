PROGRAM := $(shell basename `readlink -f .`)

BIN_DIR := ./bin
OUT_DIR := ./build
SRC_DIR := ./src
INCLUDE_DIR := $(SRC_DIR)

IGNORE_PATTERN := /_

TARGET := $(OUT_DIR)/$(PROGRAM)
SRCES = $(shell find * -name *.c | grep -v $(IGNORE_PATTERN))
OBJS := $(addprefix $(OUT_DIR)/, $(subst ./, , $(patsubst %.c, %.o, $(SRCES))))
DEPENDS := $(OUT_DIR)/depend.inc

COMPILER := gcc
C_FLAGS := -Wall -O2
LD_FLAGS := -lX11
INCLUDE_FLAGS := -I$(INCLUDE_DIR)

build: prepare depend $(TARGET) publish

dev: build start

start:
	$(BIN_DIR)/$(PROGRAM)

prepare:
	@[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)
	@[ -d $(OUT_DIR) ] || mkdir -p $(OUT_DIR)
	@make clean

clean:
	-rm -f $(OBJS) $(DEPENDS) $(TARGET) $(BIN_DIR)/$(PROGRAM)

depend: $(OBJS)
	$(RM) $(OUT_DIR)/depend.inc
	-@ for i in $^; do cpp -MM $$i | sed "s/\.*\.c//g" >> $(OUT_DIR)/depend.inc; done

$(TARGET): $(OBJS)
	$(COMPILER) -o $@ $^ $(LD_FLAGS)

$(OUT_DIR)/%.o:%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(COMPILER) $(C_FLAGS) $(INCLUDE_FLAGS) -o $@ -c $<

publish:
	-cp $(TARGET) $(BIN_DIR)/$(PROGRAM)

-include $(DEPENDS)
