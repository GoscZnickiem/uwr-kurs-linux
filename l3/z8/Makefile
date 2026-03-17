# ==================================================
# Portable makefile for compilation on platforms:
#     Linux   /   Windows with MSYS2 using gcc
# install required packages like:
#       e.g. gcc, make,   libglfw3-dev, libglew-dev
# ==================================================

# ==================================================
# ==== TARGET ARCHITECTURE =========================
# ==================================================
UNAMES := $(shell uname -o)
ifeq      (${UNAMES},GNU/Linux)
	COMPILER_FLAGS =
	LINKFLAGS = -lc
	EXE =
else ifeq (${UNAMES},Msys)
	COMPILER_FLAGS = -I /msys64/mingw64/include/
	LINKFLAGS = -lc
	EXE = .exe
	COMPILER_FLAGS += -DARCH_MSYS=${UNAMES}
else
	TARGET = unknown-target
defaulterror:
	echo Unknown target architecture: uname -s
endif
# ==================================================

# ==================================================
# ==== PROJECT CONFIGURATION =======================
# ==================================================
APP_NAME = my_shell
COMPILER = gcc

COMPILER_FLAGS += -std=c23 -Wall -Wextra -Wpedantic -Wshadow -Wunused -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wimplicit-fallthrough -Wmisleading-indentation
# COMPILER_FLAGS += -Werror	# possibly not helpful
DEBUG_FLAGS = -g

SRC_DIR = src
OBJ_DIR = bin
DEB_DIR = debug

TARGET_NAME := $(APP_NAME)$(EXE)
TARGET := $(TARGET_NAME)
TARGET_DEBUG := $(DEB_DIR)/$(TARGET_NAME)

SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRCS:.c=.o))
OBJS_DEB := $(patsubst $(SRC_DIR)/%, $(DEB_DIR)/%, $(SRCS:.c=.o))
# ==================================================

# ==================================================
# ==== BUILD TARGETS ===============================
# ==================================================
default: release

$(TARGET): $(OBJS)
	$(COMPILER) $(OBJS) -o $@ $(LINKFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c*
	@mkdir -p $(dir $@)
	$(COMPILER) $(COMPILER_FLAGS) -c -o $@ $<
# ==================================================

# ==================================================
# ==== DEBUG TARGETS ===============================
# ==================================================
$(TARGET_DEBUG): $(OBJS_DEB)
	$(COMPILER) $(OBJS_DEB) -o $@ $(LINKFLAGS)

$(DEB_DIR)/%.o: $(SRC_DIR)/%.c*
	@mkdir -p $(dir $@)
	$(COMPILER) $(COMPILER_FLAGS) $(DEBUG_FLAGS) -c -o $@ $<
# ==================================================

# ==================================================
# ==== PHONY TARGETS ===============================
# ==================================================
.PHONY: makedir
makedir:
	@mkdir -p $(OBJ_DIR) $(DEB_DIR)

.PHONY: release
release: makedir $(TARGET)

.PHONY: debug
debug: makedir $(TARGET_DEBUG)

.PHONY: clean
clean:
	@rm -fr $(OBJ_DIR) $(OBJS_DEB)

.PHONY: clear
clear:
	@rm -fr $(OBJ_DIR) $(OBJS_DEB) $(TARGET) $(DEB_DIR)
# ==================================================
