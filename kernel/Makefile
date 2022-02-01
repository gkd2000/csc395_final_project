CC := clang -target x86_64-elf
LD := x86_64-elf-ld

CFLAGS := --std=c17 -Wall -O2 -I. -ffreestanding -nostdlib -fno-stack-protector -fno-pic -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -MMD -MP

LDFLAGS := -nostdlib -static

OUT := obj

SRC := $(wildcard *.c)
OBJ := $(patsubst %.c, $(OUT)/%.o, $(SRC))
DEP := $(patsubst %.c, $(OUT)/%.d, $(SRC))

.PHONY: all
all: kernel.elf

.PHONY: clean
clean:
	rm -rf kernel.elf $(OUT)

.PHONY: run
run:
	$(MAKE) -C .. run

kernel.elf: $(OBJ) linker.ld
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $(OBJ)

$(OBJ): $(OUT)/%.o: %.c
	@mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEP)
