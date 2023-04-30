PROGRAM = $(shell basename `readlink -f .`)

BIN_DIR = ./bin
OUT_DIR = ./build
SRC_DIR = ./src
INCLUDE_DIR = $(SRC_DIR)

IGNORE_PATTERN = /_

TARGET = $(OUT_DIR)/$(PROGRAM)
SRCES = $(shell find * -name *.c | grep -v $(IGNORE_PATTERN))
OBJS = $(addprefix $(OUT_DIR)/, $(subst ./, , $(patsubst %.c, %.o, $(SRCES))))
DEPENDS = $(OBJS:.o=.d)

COMPILER = gcc
C_FLAGS = -Wall -Wextra -Werror -pedantic-errors -MMD -MP -O2
LD_FLAGS = -lX11
INCLUDE_FLAGS = -I$(INCLUDE_DIR)

.PHONY: build
build: prepare $(TARGET)
	@if [ ! -e $(BIN_DIR)/$(PROGRAM) -o $(TARGET) -nt $(BIN_DIR)/$(PROGRAM) ]; then \
		cp $(TARGET) $(BIN_DIR)/$(PROGRAM); \
	else \
		echo "No need to copy '$(TARGET)' to '$(BIN_DIR)/$(PROGRAM)'."; \
	fi

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
	@if [ -d $(BIN_DIR) ] || [ -d $(OUT_DIR) ]; then \
		rm -rf $(BIN_DIR) $(OUT_DIR); \
	else \
		echo "No need to clean."; \
	fi

$(TARGET): $(OBJS)
	$(COMPILER) -o $@ $^ $(LD_FLAGS)

$(OUT_DIR)/%.o:%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(COMPILER) $(C_FLAGS) $(INCLUDE_FLAGS) -o $@ -c $<

-include $(DEPENDS)
