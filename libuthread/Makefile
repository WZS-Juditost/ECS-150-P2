# Target library
lib := libuthread.a

all: $(lib)

# Don't print the commands unless explicitly requested with `make V=1`
ifneq ($(V),1)
Q = @
V = 0
endif

# Current directory
CUR_PWD := $(shell pwd)

# Define compilation toolchain
CC	= gcc
AA	= ar

# General gcc options
CFLAGS	:= -Wall -Wextra -Werror
CFLAGS	+= -pipe
## Debug flag
ifneq ($(D),1)
CFLAGS	+= -O2
else
CFLAGS	+= -g
endif
## Dependency generation
CFLAGS	+= -MMD

# Application objects to compile
objs := context.o preempt.o queue.o sem.o uthread.o

# Include dependencies
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

# Generic rule for compiling A file
libuthread.a: $(objs)
	@echo "AA	$@"
	$(Q)$(AA) -rcs $@ $^

# Generic rule for compiling objects
%.o: %.c
	@echo "CC	$@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

# Cleaning rule
clean:
	@echo "CLEAN	$(CUR_PWD)"
	$(Q)rm -rf $(lib) $(objs) $(deps)
