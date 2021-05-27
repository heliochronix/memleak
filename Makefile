# General definitions
TARGET = test_malloc
CFLAGS = -Wall -g -Og
LDFLAGS = -Wl,--wrap,malloc -Wl,--wrap,free

# Define application sources
SRCDIR = src
MEM_INC = include
MEM_SRC = $(SRCDIR)/mem.c	\
          $(SRCDIR)/test_malloc.c

# All sources
INC := $(MEM_INC)
CSRC := $(MEM_SRC)

# Output and dependency directories
BUILDDIR = build
DEPDIR := $(BUILDDIR)/.dep
VPATH := $(sort $(dir $(CSRC)))

OBJS := $(addprefix $(BUILDDIR)/, $(notdir $(CSRC:.c=.o)))
INCDIR := $(patsubst %,-I%,$(INC))

# Generate dependencies
CFLAGS += -MD -MP -MF $(DEPDIR)/$(@F).d

all: $(OBJS) $(TARGET)

$(OBJS): | $(BUILDDIR) $(DEPDIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(DEPDIR):
	@mkdir -p $(DEPDIR)

$(OBJS): $(BUILDDIR)/%.o : %.c $(MAKEFILE_LIST)
	$(CC) -c $(CFLAGS) -I. $(INCDIR) $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

clean:
	@rm -rf $(BUILDDIR) $(TARGET)
