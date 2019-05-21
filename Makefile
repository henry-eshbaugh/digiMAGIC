#-------------------------
# COMPILER FLAGS 
#-------------------------
INC_DIRS += ./include
INC_DIRS += ./libopencm3/include
INC = $(addprefix -I,$(INC_DIRS))
ARCH_FLAGS=-mthumb -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -ffast-math
CFLAGS=-O2 -std=c99 -Wall -Wextra -Warray-bounds -Wno-unused-parameter $(ARCH_FLAGS) $(INC) -DSTM32F7 -DUSE_HAL_DRIVER

#------------------------- 
# LINKER FLAGS
#------------------------- 
# device is actually an f722, but the f723 is compatible
DEVICE = stm32f723rc
LDLIBS += c # C standard library
LDLIBS += m # mathematical routines
LDLIBS += nosys # no system libs
LDLIBS += opencm3_stm32f7 # opencm3
LDFLAGS=-Wl,--gc-sections --static -nostartfiles -Tstm32f7.ld 

.PHONY: all flash clean tty cat

CC=arm-none-eabi-gcc
OBJ = bin/ads8681.o bin/wavelet.o bin/ad5689.o bin/main.o bin/ft230x.o

# build object files
bin/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

stm32f7.ld:
	python libopencm3/scripts/genlink.py libopencm3/ld/devices.data $(DEVICE) DEFS
	echo

# link object files
all: $(OBJ) stm32f7.ld
	$(CC) -o bin/digimag.elf $^ $(CFLAGS) $(LDFLAGS)

bin/digimag.bin: bin/digimag.elf
	arm-none-eabi-objcopy -O binary bin/digimag.elf bin/digimag.bin

flash: bin/digimag.bin
	st-flash write bin/digimag.bin 0x8000000
	
clean:
	rm -f bin/*.o bin/*.elf bin/*.bin
