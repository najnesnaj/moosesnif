###############################################################################
# Executables

PREFIX		?= arm-none-eabi

CC		:= $(PREFIX)-gcc
CXX		:= $(PREFIX)-g++
LD		:= $(PREFIX)-gcc
AR		:= $(PREFIX)-ar
AS		:= $(PREFIX)-as
SIZE		:= $(PREFIX)-size
OBJCOPY		:= $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump
GDB		:= $(PREFIX)-gdb
STFLASH		= $(shell which st-flash)
STYLECHECK	:= /checkpatch.pl
STYLECHECKFLAGS	:= --no-tree -f --terse --mailback
STYLECHECKFILES	:= $(shell find . -name '*.[ch]')
OPT		:= -Os
DEBUG		:= -ggdb3
CSTD		?= -std=c99

###############################################################################
# Source files

SRCDIR := src/
CPP_SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
C_SOURCES := $(shell find $(SRCDIR) -type f -name *.c)
OBJS += $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CPP_SOURCES:.cpp=.o))
OBJS += $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(C_SOURCES:.c=.o))

# Printf
OBJS += ./printf/printf.o
DEFS += -I./printf/

ifeq ($(V),1)
$(info Using $(OPENCM3_DIR) path to library)
endif

define ERR_DEVICE_LDSCRIPT_CONFLICT
You can either specify DEVICE=blah, and have the LDSCRIPT generated,
or you can provide LDSCRIPT, and ensure CPPFLAGS, LDFLAGS and LDLIBS
all contain the correct values for the target you wish to use.
You cannot provide both!
endef

ifeq ($(strip $(DEVICE)),)
# Old style, assume LDSCRIPT exists
DEFS		+= -I$(OPENCM3_DIR)/include \
			   -I./include/
USER_DEFS       +=
LDFLAGS		+= -L$(OPENCM3_DIR)/lib
LDLIBS		+= -l$(LIBNAME)
LDSCRIPT	?= $(BINARY).ld
else
# New style, assume device is provided, and we're generating the rest.
ifneq ($(strip $(LDSCRIPT)),)
$(error $(ERR_DEVICE_LDSCRIPT_CONFLICT))
endif
include $(OPENCM3_DIR)/mk/genlink-config.mk
endif

OPENCM3_SCRIPT_DIR = $(OPENCM3_DIR)/scripts

###############################################################################
# C flags

TGT_CFLAGS	+= $(OPT) $(CSTD) $(DEBUG)
TGT_CFLAGS	+= $(ARCH_FLAGS)
TGT_CFLAGS	+= -Wextra -Wshadow -Wimplicit-function-declaration
TGT_CFLAGS	+= -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
TGT_CFLAGS	+= -fno-common -ffunction-sections -fdata-sections
TGT_CFLAGS	+= -fno-builtin -fno-exceptions -fno-unwind-tables


###############################################################################
# C++ flags

TGT_CXXFLAGS	+= $(OPT) $(CXXSTD) $(DEBUG)
TGT_CXXFLAGS	+= $(ARCH_FLAGS)
TGT_CXXFLAGS	+= -Wextra -Wshadow -Wredundant-decls -Weffc++
TGT_CXXFLAGS	+= -fno-common -ffunction-sections -fdata-sections -fno-threadsafe-statics
TGT_CXXFLAGS	+= -fno-builtin -fno-rtti -fno-exceptions -fno-unwind-tables -fno-use-cxa-atexit
TGT_CXXFLAGS	+= -std=c++11

###############################################################################
# C & C++ preprocessor common flags

TGT_CPPFLAGS	+= -MD
TGT_CPPFLAGS	+= -Wall -Wundef
TGT_CPPFLAGS	+= $(DEFS)
TGT_CPPFLAGS	+= $(USER_DEFS)

###############################################################################
# Linker flags

TGT_LDFLAGS		+= --static -nostartfiles
TGT_LDFLAGS		+= -T$(LDSCRIPT)
TGT_LDFLAGS		+= $(ARCH_FLAGS) $(DEBUG)
TGT_LDFLAGS		+= -Wl,-Map=$(*).map -Wl,--cref
TGT_LDFLAGS		+= -Wl,--gc-sections -Wl,--no-wchar-size-warning
ifeq ($(V),99)
TGT_LDFLAGS		+= -Wl,--print-gc-sections
endif

###############################################################################
# Used libraries

LDLIBS		+= -Wl,--start-group -lc -lgcc -lstdc++ -lnosys -Wl,--end-group -lm

###############################################################################
###############################################################################
###############################################################################

.SUFFIXES: .elf .bin .hex .srec .list .map .images
.SECONDEXPANSION:
.SECONDARY:

all: elf bin size

size: $(BINARY).size
elf: $(BINARY).elf
bin: $(BINARY).bin
hex: $(BINARY).hex
srec: $(BINARY).srec
list: $(BINARY).list
GENERATED_BINARIES=$(BINARY).elf $(BINARY).bin $(BINARY).hex $(BINARY).srec $(BINARY).list $(BINARY).map

images: $(BINARY).images
flash: $(BINARY).flash

# Either verify the user provided LDSCRIPT exists, or generate it.
ifeq ($(strip $(DEVICE)),)
$(LDSCRIPT):
    ifeq (,$(wildcard $(LDSCRIPT)))
        $(error Unable to find specified linker script: $(LDSCRIPT))
    endif
else
include $(OPENCM3_DIR)/mk/genlink-rules.mk
endif

# Define a helper macro for debugging make errors online
# you can type "make print-OPENCM3_DIR" and it will show you
# how that ended up being resolved by all of the included
# makefiles.
print-%:
	@echo $*=$($*)

%.images: %.bin %.hex %.srec %.list %.map
	@#printf "*** $* images generated ***\n"

%.bin: %.elf
	@#printf "  OBJCOPY $(*).bin\n"
	$(Q)$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.hex: %.elf
	@#printf "  OBJCOPY $(*).hex\n"
	$(Q)$(OBJCOPY) -Oihex $(*).elf $(*).hex

%.srec: %.elf
	@#printf "  OBJCOPY $(*).srec\n"
	$(Q)$(OBJCOPY) -Osrec $(*).elf $(*).srec

%.list: %.elf
	@#printf "  OBJDUMP $(*).list\n"
	$(Q)$(OBJDUMP) -S $(*).elf > $(*).list

%.elf %.map: $(OBJS) $(LDSCRIPT)
	@#printf "  LD      $(*).elf\n"
	$(Q)$(LD) $(TGT_LDFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(*).elf

%.o: %.c
	@#printf "  CC      $(*).c\n"
	$(Q)$(CC) $(TGT_CFLAGS) $(CFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $(*).o -c $(*).c

%.o: %.cxx
	@#printf "  CXX     $(*).cxx\n"
	$(Q)$(CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $(*).o -c $(*).cxx

%.o: %.cpp
	@#printf "  CXX     $(*).cpp\n"
	$(Q)$(CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $(*).o -c $(*).cpp

%.size: %.elf
	@echo "Output code size:"
	@$(SIZE) -A -d $(*).elf | egrep 'text|data|bss' | awk ' \
    function human(x) { \
        if (x<1000) {return x} else {x/=1024} \
        s="kMGTEPZY"; \
        while (x>=1000 && length(s)>1) \
            {x/=1024; s=substr(s,2)} \
        return int(x+0.5) substr(s,1,1) \
    } \
	{printf("%10s %8s\n", $$1, human($$2))} \
'

clean:
	@#printf "  CLEAN\n"
	$(Q)$(RM) $(GENERATED_BINARIES) generated.* $(OBJS) $(OBJS:%.o=%.d)

stylecheck: $(STYLECHECKFILES:=.stylecheck)
styleclean: $(STYLECHECKFILES:=.styleclean)

# the cat is due to multithreaded nature - we like to have consistent chunks of text on the output
%.stylecheck: %
	$(Q)$(OPENCM3_SCRIPT_DIR)$(STYLECHECK) $(STYLECHECKFLAGS) $* > $*.stylecheck; \
		if [ -s $*.stylecheck ]; then \
			cat $*.stylecheck; \
		else \
			rm -f $*.stylecheck; \
		fi;

%.styleclean:
	$(Q)rm -f $*.stylecheck;

%.flash: %.elf
	@printf "  GDB   $(*).elf (flash)\n"
	$(GDB) --batch \
		   -ex 'target extended-remote $(BMP_PORT)' \
		   -x bmp_flash.scr \
		   $(*).elf

option_bytes:
	$(GDB) --batch \
		-ex 'target extended-remote $(BMP_PORT)' \
		-ex 'monitor version' \
		-ex 'monitor swdp_scan' \
		-ex 'attach 1' \
		-ex 'set mem inaccessible-by-default off' \
		-ex 'mon option 0x1FFFF802 0x01FE'  # Enable IWDG in hardware

option_bytes_reset:
	$(GDB) --batch \
		-ex 'target extended-remote $(BMP_PORT)' \
		-ex 'monitor version' \
		-ex 'monitor swdp_scan' \
		-ex 'attach 1' \
		-ex 'set mem inaccessible-by-default off' \
		-ex 'mon option erase' \
		-ex 'mon option 0x1FFFF800 0x55AA' \
		-ex 'mon option 0x1FFFF802 0x00FF' \
		-ex 'mon option 0x1FFFF804 0x00FF' \
		-ex 'mon option 0x1FFFF806 0x00FF' \
		-ex 'mon option 0x1FFFF808 0x00FF' \
		-ex 'mon option 0x1FFFF80A 0x00FF' \
		-ex 'mon option 0x1FFFF80C 0x00FF' \
		-ex 'mon option 0x1FFFF80E 0x00FF'

.PHONY: images clean stylecheck styleclean elf bin hex srec list

-include $(OBJS:.o=.d)
