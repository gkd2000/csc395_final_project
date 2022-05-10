CC := clang -target x86_64-elf
LD := x86_64-elf-ld

CFLAGS := --std=c17 -Wall -O2 -I. -isystem ../stdlib -ffreestanding -nostdlib -fno-stack-protector -fno-pic -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-red-zone -mcmodel=medium -MMD -MP

LDFLAGS := -nostdlib -static -L../stdlib -lc

OUT := obj

SRC := $(wildcard *.c)
ASM := $(wildcard *.s)
C_OBJ := $(patsubst %.c, $(OUT)/%.o, $(SRC))
S_OBJ := $(patsubst %.s, $(OUT)/%.o, $(ASM))
DEP := $(patsubst %.c, $(OUT)/%.d, $(SRC))

.PHONY: all
all: paint

.PHONY: clean
clean:
	rm -rf paint $(OUT)

paint: $(C_OBJ) $(S_OBJ) linker.ld
	$(LD) -T linker.ld -o $@ $(C_OBJ) $(S_OBJ) $(LDFLAGS)

$(C_OBJ): $(OUT)/%.o: %.c
	@mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -c $< -o $@

$(S_OBJ): $(OUT)/%.o: %.s
	@mkdir -p `dirname $@`
	$(CC) -c $< -o $@

-include $(DEP)
