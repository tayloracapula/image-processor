PROJECT_NAME := image-processor

SRCDIR := src
BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj
BINDIR := $(BUILDDIR)/bin

# Build configuration (overridable through command line)
C_STANDARD ?= c17
OPTIMISATION ?= 2
DEBUG ?= 0
STATIC ?= 0
VERBOSE ?= 0

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Compiler configuration (overridable through command line)
CC ?= gcc
MINGW_CC ?= x86_64-w64-mingw32-gcc

# Build flags 
BASE_CFLAGS := -Wall -Wextra -std=$(C_STANDARD)
OPT_FLAGS := -O$(OPTIMISATION)

ifeq ($(DEBUG),1)
	CFLAGS := $(BASE_CFLAGS) -g -DDEBUG
else
	CFLAGS := $(BASE_CFLAGS) $(OPT_FLAGS)
endif

ifeq ($(STATIC),1)
	LDFLAGS += -static
endif

ifeq ($(VERBOSE),1)
	Q :=
else
	Q := @
endif

INCLUDES ?= -I./include
LIBS ?= -lm

WIN_LDFLAGS := -static
WIN_LIBS := -lmingw32 -lmsvcrt -lm

LINUX_TARGET := $(BINDIR)/$(PROJECT_NAME)
WINDOWS_TARGET := $(BINDIR)/$(PROJECT_NAME).exe

# Default target
.PHONY: all
all: linux

# Linux build
.PHONY: linux
linux: $(LINUX_TARGET)

$(LINUX_TARGET): $(OBJECTS) | $(BINDIR)
	$(Q)$(CC) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBS)
	@echo "Linux build complete: $@"

# Windows cross compile
.PHONY: windows
windows: $(WINDOWS_TARGET)

$(WINDOWS_TARGET): CC := $(MINGW_CC)
$(WINDOWS_TARGET): LDFLAGS := $(WIN_LDFLAGS)
$(WINDOWS_TARGET): LIBS := $(WIN_LIBS)
$(WINDOWS_TARGET): $(OBJECTS) | $(BINDIR)
	$(Q)$(MINGW_CC) $(OBJECTS) -o $@ $(WIN_LDFLAGS) $(WIN_LIBS)
	@echo "Windows build complete: $@"

# Debug build 
.PHONY: debug
debug:
	$(MAKE) DEBUG=1

# Object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(Q)$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Compiled: $<"

# Create directories
$(OBJDIR):
	$(Q)mkdir -p $(OBJDIR)

$(BINDIR):
	$(Q)mkdir -p $(BINDIR)


.PHONY: check
check:
	@echo "Checking for required tools..."
	@command -v $(CC) >/dev/null 2>&1 || { echo "$(CC) not found!"; exit 1; }
	@command -v $(MINGW_CC) >/dev/null 2>&1 || { echo "MinGW not found! Install with: sudo apt install gcc-mingw-w64"; exit 1; }
	@echo "All tools available!"

# Show current configuration
.PHONY: config
config:
	@echo "Build Configuration:"
	@echo "  PROJECT_NAME: $(PROJECT_NAME)"
	@echo "  VERSION: $(VERSION)"
	@echo "  C_STANDARD: $(C_STANDARD)"
	@echo "  OPTIMIZATION: $(OPTIMIZATION)"
	@echo "  DEBUG: $(DEBUG)"
	@echo "  STATIC: $(STATIC)"
	@echo "  VERBOSE: $(VERBOSE)"
	@echo "  CC: $(CC)"
	@echo "  CFLAGS: $(CFLAGS)"
	@echo "  INCLUDES: $(INCLUDES)"
	@echo "  LIBS: $(LIBS)"

.PHONY: clean
clean:
	$(Q)rm -rf $(BUILDDIR)
	@echo "Cleaned build directory" $(BUILDDIR)
