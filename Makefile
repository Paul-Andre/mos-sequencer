#https://gist.github.com/yorickvP/3171926

ifdef VERBOSE
	Q =
	E = @true 
else
	Q = @
	E = @echo 
endif

CFILES := $(shell find src -mindepth 1 -maxdepth 4 -name "*.c")
CXXFILES := $(shell find src -mindepth 1 -maxdepth 4 -name "*.cpp")

INFILES := $(CFILES) $(CXXFILES)

OBJFILES := $(CXXFILES:src/%.cpp=%) $(CFILES:src/%.c=%)
DEPFILES := $(CXXFILES:src/%.cpp=%) $(CFILES:src/%.c=%)
OFILES := $(OBJFILES:%=obj/%.o)

BINFILE = sequencer

COMMONFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -lSDL2 -lm

ifdef DEBUG
	COMMONFLAGS := $(COMMONFLAGS) -g
else
	COMMONFLAGS := $(COMMONFLAGS) -O2
endif
CFLAGS = $(COMMONFLAGS) --std=gnu99
CXXFLAGS = $(COMMONFLAGS) --std=gnu++0x
DEPDIR = deps
all: $(BINFILE)
ifeq ($(MAKECMDGOALS),)
-include Makefile.dep
endif
ifneq ($(filter-out clean, $(MAKECMDGOALS)),)
-include Makefile.dep
endif

CC = gcc
CXX = g++


-include Makefile.local

.PHONY: clean all depend
.SUFFIXES:
obj/%.o: src/%.c
	$(E)C-compiling $<
	$(Q)if [ ! -d `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(Q)$(CC) -o $@ -c $< $(CFLAGS)
obj/%.o: src/%.cpp
	$(E)C++-compiling $<
	$(Q)if [ ! -d `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(Q)$(CXX) -o $@ -c $< $(CXXFLAGS)
Makefile.dep: $(CFILES) $(CXXFILES)
	$(E)Depend
	$(Q)for i in $(^); do $(CXX) $(CXXFLAGS) -MM "$${i}" -MT obj/`basename $${i%.*}`.o; done > $@

	
$(BINFILE): $(OFILES)
	$(E)Linking $@
	$(Q)$(CXX) -o $@ $(OFILES) $(LDFLAGS)
clean:
	$(E)Removing files
	$(Q)rm -f $(BINFILE) obj/* Makefile.dep

