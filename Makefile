# Vulcalien's Library Makefile
# version 0.2.0
#
# One Makefile for Unix and Windows
# Made for the 'gcc' compiler
#
# This Makefile can create both
# Static and Shared libraries

# === DETECT OS ===
ifeq ($(OS),Windows_NT)
	CURRENT_OS := WINDOWS
else
	CURRENT_OS := UNIX
endif
TARGET_OS := $(CURRENT_OS)

# ========= EDIT HERE =========
OUT_FILENAME := libcliscreen

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

SRC_SUBDIRS :=

CC := gcc

CPPFLAGS := -Iinclude -MMD -MP

CFLAGS_STATIC := -Wall -pedantic
CFLAGS_SHARED := -fPIC -fvisibility=hidden -Wall -pedantic

ifeq ($(TARGET_OS),UNIX)
	# UNIX
	LDFLAGS := -shared -Llib
	LDLIBS  :=
else ifeq ($(TARGET_OS),WINDOWS)
	ifeq ($(CURRENT_OS),WINDOWS)
		# WINDOWS
		LDFLAGS := -shared -Llib
		LDLIBS  :=
	else ifeq ($(CURRENT_OS),UNIX)
		# UNIX to WINDOWS cross-compile
		CC := x86_64-w64-mingw32-gcc

		LDFLAGS := -shared -Llib
		LDLIBS  :=
	endif
endif
# =============================

# === OS SPECIFIC ===
ifeq ($(TARGET_OS),UNIX)
	OBJ_EXT    := .o
	STATIC_EXT := .a
	SHARED_EXT := .so
else ifeq ($(TARGET_OS),WINDOWS)
	OBJ_EXT    := .obj
	STATIC_EXT := .a
	SHARED_EXT := .dll
endif

ifeq ($(CURRENT_OS),UNIX)
	MKDIR      := mkdir
	MKDIRFLAGS := -p

	RM      := rm
	RMFLAGS := -rfv
else ifeq ($(CURRENT_OS),WINDOWS)
	MKDIR      := mkdir
	MKDIRFLAGS :=

	RM      := rmdir
	RMFLAGS := /Q /S
endif

# === OTHER ===
SRC := $(wildcard $(SRC_DIR)/*.c)\
       $(foreach DIR,$(SRC_SUBDIRS),$(wildcard $(SRC_DIR)/$(DIR)/*.c))

OBJ_STATIC_DIR := $(OBJ_DIR)/static
OBJ_SHARED_DIR := $(OBJ_DIR)/shared

OBJ_STATIC := $(SRC:$(SRC_DIR)/%.c=$(OBJ_STATIC_DIR)/%$(OBJ_EXT))
OBJ_SHARED := $(SRC:$(SRC_DIR)/%.c=$(OBJ_SHARED_DIR)/%$(OBJ_EXT))

OUT_STATIC := $(BIN_DIR)/$(OUT_FILENAME)$(STATIC_EXT)
OUT_SHARED := $(BIN_DIR)/$(OUT_FILENAME)$(SHARED_EXT)

OBJ_STATIC_DIRECTORIES := $(OBJ_STATIC_DIR)\
                          $(foreach DIR,$(SRC_SUBDIRS),$(OBJ_STATIC_DIR)/$(DIR))
OBJ_SHARED_DIRECTORIES := $(OBJ_SHARED_DIR)\
                          $(foreach DIR,$(SRC_SUBDIRS),$(OBJ_SHARED_DIR)/$(DIR))

# === TARGETS ===
.PHONY: all build-static build-shared clean

all: build-static build-shared
build-static: $(OUT_STATIC)
build-shared: $(OUT_SHARED)

clean:
	@$(RM) $(RMFLAGS) $(BIN_DIR) $(OBJ_DIR)

$(OUT_STATIC): $(OBJ_STATIC) | $(BIN_DIR)
	$(AR) rcs $@ $^

$(OUT_SHARED): $(OBJ_SHARED) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_STATIC_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%.c | $(OBJ_STATIC_DIRECTORIES)
	$(CC) $(CPPFLAGS) $(CFLAGS_STATIC) -c $< -o $@

$(OBJ_SHARED_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%.c | $(OBJ_SHARED_DIRECTORIES)
	$(CC) $(CPPFLAGS) $(CFLAGS_SHARED) -c $< -o $@

$(BIN_DIR) $(OBJ_STATIC_DIRECTORIES) $(OBJ_SHARED_DIRECTORIES):
	$(MKDIR) $(MKDIRFLAGS) "$@"

-include $(OBJ_STATIC:$(OBJ_EXT)=.d)
-include $(OBJ_SHARED:$(OBJ_EXT)=.d)
