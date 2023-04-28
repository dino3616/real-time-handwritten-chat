PROGRAM := $(shell basename `readlink -f .`)

BIN_DIR := ./bin
OUT_DIR := ./build
SRC_DIR := ./src
INCLUDE_DIR := $(SRC_DIR)

IGNORE_PATTERN := /_

TARGET := $(OUT_DIR)/$(PROGRAM)
SRCES = $(shell find * -name *.c | grep -v $(IGNORE_PATTERN))
OBJS := $(addprefix $(OUT_DIR)/, $(subst ./, , $(patsubst %.c, %.o, $(SRCES))))
DEPENDS := $(OBJS:.o=.d)

COMPILER := gcc
C_FLAGS := -Wall -Wextra -Werror -pedantic-errors -MMD -MP -O2
LD_FLAGS := -lX11
INCLUDE_FLAGS := -I$(INCLUDE_DIR)

.PHONY: build
build: prepare $(TARGET) publish

.PHONY: dev
dev: build start

.PHONY: start
start:
	$(BIN_DIR)/$(PROGRAM)

.PHONY: prepare
prepare:
	@[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)
	@[ -d $(OUT_DIR) ] || mkdir -p $(OUT_DIR)

.PHONY: clean
clean:
	$(RM) -rf $(BIN_DIR) $(OUT_DIR)

$(TARGET): $(OBJS)
	$(COMPILER) -o $@ $^ $(LD_FLAGS)

$(OUT_DIR)/%.o:%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(COMPILER) $(C_FLAGS) $(INCLUDE_FLAGS) -o $@ -c $<

.PHONY: publish
publish:
	-cp $(TARGET) $(BIN_DIR)/$(PROGRAM)

-include $(DEPENDS)
