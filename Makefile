PREFIX := arm-none-eabi-
ASFLAGS:=-g
AS := $(PREFIX)as
CC := $(PREFIX)gcc
OBJDUMP := $(PREFIX)objdump
OBJCOPY := $(PREFIX)objcopy
FLAGS := -Wall -Werror -Wextra -g -std=gnu99 -O1
CFLAGS := $(FLAGS)
LINKFLAGSBOOT := -Wl,-T,ldscript_boot -nostdlib
LINKFLAGS0X3000 := -Wl,-T,ldscript0x3000 -nostdlib
LINKFLAGS := -Wl,-T,ldscript -nostdlib
TIMER := timers.o
LED := leds.o
SERIAL := serial.o
FILES := $(TIMER) $(LED) $(SERIAL)
REG_H := registers.h

all : main timer_test program0x3000 timer_test.bin hello.lst

timer_test : timer_test.o $(TIMER) $(LED) crt0.o
	$(CC) $(FLAGS) $(LINKFLAGS) $^ -o $@

%.bin : %
	$(OBJCOPY) -O binary $^ $@

program0x3000 : program0x3000.o $(TIMER) $(LED) crt0.o initialisation.o
	$(CC) $(FLAGS) $(LINKFLAGS0X3000) $^ -o $@

main : $(FILES) main.o crt0.o bootloader.o initialisation.o
	$(CC) $(FLAGS) $(LINKFLAGSBOOT) $^ -o $@

hello.lst : main
	$(OBJDUMP) -d -S $< > $@

$(TIMER) : $(REG_H)
$(LED) : $(REG_H)
$(SERIAL) : $(REG_H)

.PHONY : clean all

clean :
	rm -f *.o
	rm -f timer_test
	rm -f program0x3000
	rm -f main
	rm -f hello.lst
	rm -f *.bin
