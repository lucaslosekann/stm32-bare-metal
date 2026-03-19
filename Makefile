CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

ROOTDIR := $(CURDIR)
LIBDIR = ${ROOTDIR}/lib

CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
	-Wformat-truncation -fno-common -Wconversion \
	-g3 -Og -ffunction-sections -fdata-sections -I. -I${LIBDIR} \
	-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)

LDFLAGS ?= -Tf411.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SRCS = main.c $(wildcard ${LIBDIR}/*.c)
OBJS = $(SRCS:.c=.o)

all: firmware.bin

firmware.elf: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary $< $@

flash: firmware.bin
	sudo st-flash --reset write $< 0x8000000

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf firmware.*
	rm -rf $(OBJS)

.PHONY: all clean flash