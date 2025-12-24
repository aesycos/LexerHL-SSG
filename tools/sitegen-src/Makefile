# =========================
# Project configuration
# =========================

PROJECT := sitegen

# User-selectable compiler
CXX ?= g++
STD := -std=c++20

SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

DEBUG_DIR := $(BUILD_DIR)/debug
RELEASE_DIR := $(BUILD_DIR)/release

# =========================
# Compiler detection
# =========================

IS_CLANG := $(findstring clang,$(shell $(CXX) --version 2>/dev/null))
IS_GCC   := $(findstring GCC,$(shell $(CXX) --version 2>/dev/null))

# =========================
# Optional lexers
# =========================
# Usage:
#   make
#   make LEXERS=cpp
#   make LEXERS="cpp js"
#   make all LEXERS="cpp js"

LEXERS ?=
ALL_LEXERS := cpp js

# Enable all lexers when building 'all'
ifneq (,$(filter all,$(MAKECMDGOALS)))
    LEXERS := $(ALL_LEXERS)
endif

LEXER_SRC =
LEXER_DEFS :=

LEXER_SRC += $(if $(filter cpp,$(LEXERS)),$(SRC_DIR)/CPPLexer.cpp)
LEXER_DEFS += $(if $(filter cpp,$(LEXERS)),-DLEXER_CPP)

LEXER_SRC += $(if $(filter js,$(LEXERS)),$(SRC_DIR)/JSLexer.cpp)
LEXER_DEFS += $(if $(filter js,$(LEXERS)),-DLEXER_JS)


# =========================
# Core sources
# =========================

CORE_SRC := \
    $(SRC_DIR)/lexer.cpp \
    $(SRC_DIR)/main.cpp

SRC = $(CORE_SRC) $(LEXER_SRC)

# =========================
# Include paths
# =========================

INCLUDES := -I$(INC_DIR)

# =========================
# Warnings
# =========================

WARNINGS := -Wall -Wextra -Wpedantic

ifeq ($(IS_CLANG),clang)
    WARNINGS += -Wshadow -Wconversion -Wsign-conversion
endif

ifeq ($(IS_GCC),GCC)
    WARNINGS += -Wduplicated-cond -Wlogical-op
endif

# =========================
# Sanitizers (debug only)
# =========================
# Usage:
#   make debug SAN=address
#   make debug SAN=undefined
#   make debug SAN=address,undefined

SAN ?=
SAN_FLAGS :=

ifneq ($(SAN),)
    SAN_FLAGS := -fsanitize=$(SAN) -fno-omit-frame-pointer
endif

# =========================
# Build flags
# =========================

DEBUG_FLAGS := -g -O0 -DDEBUG $(SAN_FLAGS)
RELEASE_FLAGS := -O2 -DNDEBUG

DEP_FLAGS := -MMD -MP

# =========================
# Object files
# =========================

DEBUG_OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(DEBUG_DIR)/%.o)
RELEASE_OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(RELEASE_DIR)/%.o)

# =========================
# Targets
# =========================

.PHONY: all debug release clean

all: debug release

debug: $(DEBUG_DIR)/$(PROJECT)

release: $(RELEASE_DIR)/$(PROJECT)

# =========================
# Linking
# =========================

$(DEBUG_DIR)/$(PROJECT): $(DEBUG_OBJ)
	@mkdir -p $(DEBUG_DIR)
	$(CXX) $(STD) $(WARNINGS) $(DEBUG_FLAGS) $^ -o $@

$(RELEASE_DIR)/$(PROJECT): $(RELEASE_OBJ)
	@mkdir -p $(RELEASE_DIR)
	$(CXX) $(STD) $(WARNINGS) $(RELEASE_FLAGS) $^ -o $@

# =========================
# Compilation rules
# =========================

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(STD) $(WARNINGS) $(DEBUG_FLAGS) $(INCLUDES) $(LEXER_DEFS) $(DEP_FLAGS) -c $< -o $@

$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(STD) $(WARNINGS) $(RELEASE_FLAGS) $(INCLUDES) $(LEXER_DEFS) $(DEP_FLAGS) -c $< -o $@

# =========================
# Dependency includes
# =========================

-include $(DEBUG_OBJ:.o=.d)
-include $(RELEASE_OBJ:.o=.d)

# =========================
# Clean
# =========================

clean:
	rm -rf $(BUILD_DIR)
