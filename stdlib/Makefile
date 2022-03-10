CC := clang -target x86_64-elf
AR := x86_64-elf-ar

CFLAGS := --std=c17 -Wall -O2 -I. -isystem ../stdlib -ffreestanding -nostdlib -fno-stack-protector -fno-pic -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-red-zone -mcmodel=medium -MMD -MP

OUT := obj

SRC := $(wildcard *.c)
ASM := $(wildcard *.s)
C_OBJ := $(patsubst %.c, $(OUT)/%.o, $(SRC))
S_OBJ := $(patsubst %.s, $(OUT)/%.o, $(ASM))
DEP := $(patsubst %.c, $(OUT)/%.d, $(SRC))

.PHONY: all
all: libc.a

.PHONY: clean
clean:
	rm -rf libc.a $(OUT)

libc.a: $(C_OBJ) $(S_OBJ)
	$(AR) -rc $@ $^

$(C_OBJ): $(OUT)/%.o: %.c
	@mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -c $< -o $@

$(S_OBJ): $(OUT)/%.o: %.s
	@mkdir -p `dirname $@`
	$(CC) -c $< -o $@

-include $(DEP)
