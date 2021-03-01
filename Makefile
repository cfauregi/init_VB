PROJ_NAME =main
CC=arm-none-eabi-gcc
SRCS= main.o init.o 

# ========================================================

DRAPEAU =-Wall
DRAPEAU +=-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4  -Os
DRAPEAU +=--static -nostartfiles -Tstm32f4-discovery.ld
DRAPEAU +=-lopencm3_stm32f4 -I/home/vibiscus/libopencm3/include
LDFLAGS = -L/home/vibiscus/libopencm3/lib

# ========================================================

all:$(PROJ_NAME).bin

$(PROJ_NAME).bin: $(PROJ_NAME).elf
	arm-none-eabi-objcopy -Obinary $(PROJ_NAME).elf $(PROJ_NAME).bin

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(DRAPEAU) -o $(PROJ_NAME).elf $(SRCS) $(LDFLAGS) -lopencm3_stm32f4


init.o: init.c
	$(CC) $(DRAPEAU) -c init.c


main.o: main.c
	$(CC) $(DRAPEAU) -c main.c


# ========================================================

clean:
	rm *.o $(PROJ_NAME).elf $(PROJ_NAME).bin

