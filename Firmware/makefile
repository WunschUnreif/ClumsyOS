SRCS = main.c stm32f10x_it.c startup_stm32f10x_hd.s 
HEADERS = stm32f10x_it.h stm32f10x_conf.h

########################## USER FILES BELOW ##########################
SRCS += 	usart.c os_init.c os_spot.c os_proc.c 
HEADERS += 	usart.h os_init.h os_spot.h os_proc.h 

SRCS += 	mem.c os_mem.c st_sdio_sd.c sdcard.c 
HEADERS += 	mem.h os_mem.h st_sdio_sd.h sdcard.h 

SRCS += 	diskio.c ff.c ffsystem.c ffunicode.c 
HEADERS += 	diskio.h ff.h ffconf.h

HEADERS += bsp.h os.h
########################## USER FILES ABOVE ##########################

PROJ_NAME = main

vpath %.c User User/os User/bsp User/third
vpath %.a Lib
vpath %.s Startup 
vpath %.h User User/os User/bsp User/third

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
READELF=arm-none-eabi-readelf

CFLAGS  = -g -O2 -Wall -Tstm32_flash.ld 
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m3 -mthumb-interwork
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -DSTM32F10X_HD

CFLAGS += -ILib/inc -ICMSIS -IUser -lc -IUser/bsp -IUser/os -IUser/third

OBJS = $(SRCS:.c=.o)

.PHONY: lib proj

all: lib proj

lib:
	$(MAKE) -C Lib 

proj: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -LLib -lstm32periLib
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -D -m arm -M force-thumb -b binary $(PROJ_NAME).bin > $(PROJ_NAME)_dump.asm
	$(READELF) -a $(PROJ_NAME).elf > $(PROJ_NAME).delf

SRCS : HEADERS
HEADERS:

.PHONY: clean

clean:
	rm -f *.o *.elf *.hex *.bin *.asm *.delf *.s
