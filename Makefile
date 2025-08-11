# $@ = target file
# $< = first dependency
# $^ = all dependencies

.PHONY: all run rerun echo debug clean test-env

# --- Quellen ---
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c stdlibs/*.c programs/*.c programs/dobby/*.c)
HEADERS   = $(wildcard kernel/*.h  drivers/*.h cpu/*.h stdlibs/*.h programs/*.h programs//dobby/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o cpu/interrupt.o cpu/setjmp.o cpu/longjmp.o}

# --- Default ---
all: run

# --- Revision bump (wie gehabt) ---
CURRENT_REVISION := $(shell cat version.txt)
CURRENT_DATE     := $(shell date)
NEW_REVISION     := $(shell expr $(CURRENT_REVISION) + 1)
$(shell echo $(NEW_REVISION) > version.txt)

CFLAGS := -DREVISION_NUMBER=$(NEW_REVISION) -DREVISION_DATE='"$(CURRENT_DATE)"'

# --- Build Kernel ---
kernel.bin: boot/kernel_entry.o ${OBJ_FILES}
	ld -nostdlib -m elf_i386 -o $@ -Ttext 0x10000 $^ --oformat binary

os-image.bin: boot/mbr.bin kernel.bin
	cat $^ > $@

# --- QEMU detect + CPU (32-bit Kernel auf 64-bit QEMU erzwingen) ---
QEMU32 := $(shell command -v qemu-system-i386 2>/dev/null)
QEMU64 := $(shell command -v qemu-system-x86_64 2>/dev/null)
QEMU   := $(if $(QEMU32),$(QEMU32),$(QEMU64))
CPU32  := $(if $(QEMU32),, -cpu qemu32)

# --- Audio backend auto-detect (pa -> pipewire -> alsa -> sdl) ---
# nutzt das, was dein QEMU kann. PC-Speaker wird an denselben Audiodev gehängt.
AUDIO_DRV := $(shell \
  if [ -n "$(QEMU)" ]; then \
    "$(QEMU)" -audio help 2>/dev/null | awk '/Available audio drivers:/{ok=1;next} ok{gsub(",","");print}' \
    | tr ' ' '\n' | grep -E '^(pa|pipewire|alsa|sdl)$$' | head -n1 ; \
  fi)

ifeq ($(strip $(AUDIO_DRV)),)
  $(warning Kein QEMU-Audio-Treiber erkannt. Starte ohne Sound.)
  SOUND :=
else
  AUDIODEV := -audiodev $(AUDIO_DRV),id=snd
  SOUND := $(AUDIODEV) -device sb16,audiodev=snd -machine pcspk-audiodev=snd
endif

# --- Lauf-Flags (stdvga => BGA/VBE; LFB via 0x01CE/0x01CF im Kernel setzen) ---
RUNFLAGS := -m 1024 -rtc base=2023-08-03T12:34:56 -vga std $(CPU32)

# --- Run: 1.44MB Floppy-Image booten, test.pc injizieren/extrahieren (wie bei dir) ---
run: os-image.bin
	dd if=/dev/zero of=disk_image.img bs=512 count=2880
	dd if=os-image.bin of=disk_image.img conv=notrunc
	dd if=test.pc of=disk_image.img bs=1 seek=1048576 conv=notrunc
	@if [ -z "$(QEMU)" ]; then echo "QEMU fehlt (install qemu-system-x86)"; exit 127; fi
	$(QEMU) $(RUNFLAGS) $(SOUND) -fda disk_image.img -boot a
	dd if=disk_image.img of=test.pc bs=1 skip=1048576 count=951

rerun:
	@if [ -z "$(QEMU)" ]; then echo "QEMU fehlt (install qemu-system-x86)"; exit 127; fi
	$(QEMU) $(RUNFLAGS) $(SOUND) -fda disk_image.img -boot a

echo: os-image.bin
	xxd $<

# --- Debug: GDB stub + hilfreiche Logs ---
kernel.elf: boot/kernel_entry.o ${OBJ_FILES}
	ld -nostdlib -m elf_i386 -o $@ -Ttext 0x10000 $^

debug: os-image.bin kernel.elf
	@if [ -z "$(QEMU)" ]; then echo "QEMU fehlt (install qemu-system-x86)"; exit 127; fi
	$(QEMU) $(RUNFLAGS) $(SOUND) -s -S -d guest_errors,int -fda disk_image.img -boot a &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

test-env:
	@echo "QEMU  : $(QEMU)"; \
	 echo "AUDIO : $(if $(AUDIO_DRV),$(AUDIO_DRV),none)"; \
	 $(if $(QEMU),$(QEMU) -audio help | sed -n '1,80p',echo "QEMU not found")

# --- Compile rules ---
%.o: %.c ${HEADERS}
	gcc -Wno-implicit-function-declaration -Wno-overflow -fno-PIC -nostdlib --no-pie -m32 -ffreestanding $(CFLAGS) -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@

# --- Clean ---
clean:
	$(RM) *.bin *.o *.dis *.elf
	$(RM) kernel/*.o
	$(RM) boot/*.o boot/*.bin
	$(RM) drivers/*.o
	$(RM) cpu/*.o
	$(RM) programs/*.o
	$(RM) stdlibs/*.o
