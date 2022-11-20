#
# Kurzanleitung
# =============
#
# make		-- Baut den Kernel.
# make all
#
# make install	-- Baut den Kernel und transferiert ihn auf den Server.
# 		   Das Board holt sich diesen Kernel beim nächsten Reset.
#
# make clean	-- Löscht alle erzeugten Dateien.
#


#
# Quellen
#
LSCRIPT = kernel.lds
OBJ = start.o src/drv/dbgu.o src/demo/dbgu_demo.o src/lib/print.o src/lib/printf.o src/drv/mc.o src/demo/mc_demo.o src/lib/io.o src/sys/ivt.o src/sys/init.o src/lib/interrupt_handler.o

#
# Konfiguration
#
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -Wall -Wextra -ffreestanding -mcpu=arm920t -O2 -std=c11 -fasm -g
#LIBGCC := $(shell $(CC) -print-libgcc-file-name)

DEP = $(OBJ:.o=.d)

#
# Regeln
#
.PHONY: all 
all: kernel.elf

-include $(DEP)

%.o: %.S
	$(CC) $(CFLAGS) -MMD -MP -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -o $@ -c $<

kernel.elf: $(LSCRIPT) $(OBJ)
	$(LD) -T$(LSCRIPT) -o $@ $(OBJ) $(LIBGCC)

kernel.bin: kernel.elf
	$(OBJCOPY) -Obinary --set-section-flags .bss=contents,alloc,load,data $< $@

kernel.img: kernel.bin
	mkimage -A arm -T standalone -C none -a 0x20000000 -d $< $@

.PHONY: install
install: kernel.img
	arm-install-image $<

.PHONY: clean
clean:
	rm -f kernel.elf kernel.bin kernel.img
	rm -f $(OBJ)
	rm -f $(DEP)
