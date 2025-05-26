boot.bin : stage1.o bios1.o kernel.o klib.o bios2.o
	ld -melf_i386 -T boot.ld --orphan-handling=discard $^ -o $@

%.o : %.c
	gcc -m16 -O0 --freestanding -fno-pic $(NO_CF_PROTECT) -c $(CFLAGS) $< -o $@

%.o : %.S
	as -32 $< -o $@

stage1.o : bios1.h kernel.h
kernel.o : bios1.h klib.h
klib.o   : bios2.h klib.h

boot.bin : .EXTRA_PREREQS = rt0.o boot.ld

.PHONY: clean

clean:
	rm -f *.bin *.o *~
