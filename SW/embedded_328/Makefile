# Hey Emacs, this is a -*- makefile -*-

# AVR-GCC Makefile template, derived from the WinAVR template (which
# is public domain), believed to be neutral to any flavor of "make"
# (GNU make, BSD make, SysV make)

#F_CPU = 7372800
F_CPU = 16000000
#F_CPU =  16020028

#MCU = atmega2560
#MCU = atmega128
#MCU = atmega88
MCU = atmega328p

FORMAT = ihex
#FORMAT = binary
TARGET = main

SRC = src/$(TARGET).c

SRC += src/Bsw/Uart/Uart.c
SRC += src/Bsw/Uart/Uart_Lcfg.c
SRC += src/Bsw/Gpio/Gpio.c
SRC += src/Bsw/Gpio/Gpio_Lcfg.c
SRC += src/Bsw/Adc/Adc.c
SRC += src/Bsw/Adc/Adc_Lcfg.c
SRC += src/Bsw/Ecu/EcuM.c
SRC += src/Bsw/Tools/Tools.c
SRC += src/Bsw/Dbg/Dbg.c
SRC += src/Bsw/Timer/Timer.c

SRC += src/Hal/Caliper/Caliper.c
SRC += src/Hal/Heating/Heating.c
SRC += src/Hal/R2RDac/R2RDac.c
SRC += src/Hal/HC595/HC595.c
SRC += src/Hal/Lcd/Lcd.c
SRC += src/Hal/Led/Led.c
SRC += src/Hal/RotaryEncoder/RotaryEncoder.c

SRC += src/App/App.c

LIBSRC =

INC = -I./inc -I./src/Bsw/Uart -I./src/Bsw/Adc -I./src/Bsw/Gpio -I./src/Bsw/Ecu -I./src/Bsw/Tools  -I./src/Bsw/Dbg -I./src/Bsw/Timer
INC+= -I./src/Hal/Caliper -I./src/Hal/Heating -I./src/Hal/R2RDac -I./src/Hal/HC595 -I./src/Hal/Lcd -I./src/Hal/RotaryEncoder -I./src/Hal/Led
INC+= -I./src/App
ASRC =
OPT = s

# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)UL

# Place -I options here
CINCS =


CDEBUG = -g$(DEBUG)
CWARN = -ffreestanding -pedantic -Wall -Wextra -Winit-self -Wswitch-default -Wunused-parameter -Wunknown-pragmas -Wstrict-overflow=1 -Warray-bounds -Wfloat-equal -Wdeclaration-after-statement -Wundef -Wno-endif-labels -Wshadow -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Wvla -Wvolatile-register-var -Wparentheses -g -Os -fno-strict-aliasing 
#CWARN = -ffreestanding -pedantic -Wall -Wextra -Winit-self -Wswitch-default -Wunused-parameter -Wunknown-pragmas -Wstrict-overflow=1 -Warray-bounds -Wfloat-equal -Wdeclaration-after-statement -Wundef -Wno-endif-labels -Wshadow -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Wvla -Wvolatile-register-var -Wparentheses -g -Os -fno-strict-aliasing -ffunction-sections -fdata-sections
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)
#CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CSTANDARD) $(CEXTRA)
CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CSTANDARD) $(CEXTRA) -ffunction-sections -fdata-sections


#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs


#Additional libraries.

# Minimalistic printf version
#PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

PRINTF_LIB =$(PRINTF_LIB_FLOAT)

# Minimalistic scanf version
#SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
#SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

SCANF_LIB =

MATH_LIB = -lm

# External memory options

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--defsym=__heap_start=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

#LDMAP = $(LDFLAGS) -Wl,-Map=$(TARGET).map,--cref
LDFLAGS = $(EXTMEMOPTS) $(LDMAP) $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB) -Wl,--gc-sections


# Programming support using avrdude. Settings and variables.

#AVRDUDE_PROGRAMMER = arduino
#AVRDUDE_PROGRAMMER = dragon_isp
AVRDUDE_PROGRAMMER = avrispmkII
#AVRDUDE_PORT = /dev/ttyUSB0
AVRDUDE_PORT = /dev/ttyACM1
#AVRDUDE_PORT = usb
AVRDUDE_BAUD = 115200
#AVRDUDE_BAUD = 38400

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep


# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE_COUNTER = -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
#AVRDUDE_NO_VERIFY = -V

# Increase verbosity level.  Please use this when submitting bug
# reports about avrdude. See <http://savannah.nongnu.org/projects/avrdude>
# to submit bug reports.
#AVRDUDE_VERBOSE = -v -v

#AVRDUDE_BASIC = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_BASIC = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -b $(AVRDUDE_BAUD)
AVRDUDE_FLAGS = $(AVRDUDE_BASIC) $(AVRDUDE_NO_VERIFY) $(AVRDUDE_VERBOSE) $(AVRDUDE_ERASE_COUNTER) -C /etc/avrdude.conf

CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
REMOVE_TREE = rm -rf
MV = mv -f
DOXYGEN_CALL = doxygen
DOXYGEN_CONFIG = ../doc/doxygen/doxyfile.conf
DOXYGEN_OUTPUT_DIR = ../doc/doxygen/html

# Define all object files.
OBJ = $(SRC:.c=.o)  $(LIBSRC:%.c=%.o) $(ASRC:.S=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(INC)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

ELFSIZE = $(SIZE) -A $(TARGET).elf; $(SIZE) -C --mcu=$(MCU) $(TARGET).elf

# Default target.
all: clean build program

build: elf hex eep size

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss
sym: $(TARGET).sym

# Program the device.
program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)
	#/data/programme/arduino-1.6.5/hardware/tools/avr/bin/avrdude -C/data/programme/arduino-1.6.5/hardware/tools/avr/etc/avrdude.conf -v -patmega328p -c arduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:main.hex:i



## Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
#COFFCONVERT=$(OBJCOPY) --debugging \
#--change-section-address .data-0x800000 \
#--change-section-address .bss-0x800000 \
#--change-section-address .noinit-0x800000 \
#--change-section-address .eeprom-0x810000


coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(TARGET).elf $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(TARGET).elf $(TARGET).cof


.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@



# Link: create ELF output file from object files.
$(TARGET).elf: $(OBJ)
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)


# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(ALL_CFLAGS) $< -o $@


# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Target: clean project.
clean:
	$(REMOVE) $(TARGET).hex $(TARGET).eep $(TARGET).cof $(TARGET).elf \
	$(TARGET).map $(TARGET).sym $(TARGET).lss \
	$(OBJ) $(LST) $(SRC:.c=.s) $(SRC:.c=.d)
#	$(REMOVE_TREE) $(DOXYGEN_OUTPUT_DIR)

size:
	avr-size --mcu=$(MCU) -C $(TARGET).elf

depend:
	if grep '^# DO NOT DELETE' $(MAKEFILE) >/dev/null; \
	then \
		sed -e '/^# DO NOT DELETE/,$$d' $(MAKEFILE) > \
			$(MAKEFILE).$$$$ && \
		$(MV) $(MAKEFILE).$$$$ $(MAKEFILE); \
	fi
	echo '# DO NOT DELETE THIS LINE -- make depend depends on it.' \
		>> $(MAKEFILE); \
	$(CC) -M -mmcu=$(MCU) $(CDEFS) $(CINCS) $(SRC) $(ASRC) >> $(MAKEFILE)

doxygen:
	$(DOXYGEN_CALL) $(DOXYGEN_CONFIG)

stubs:
	rm stubs/timer 	
	gcc stubs/timer.c -o stubs/timer

.PHONY:	all build elf hex eep lss sym program coff extcoff clean depend lint doxygen
