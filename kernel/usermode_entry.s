.global usermode_entry

usermode_entry:
  # Set data segment selectors (in first argument)
  mov %di, %ds
  mov %di, %es
  mov %di, %fs
  mov %di, %gs

  # Push the stack segment selector (in first argument)
  push %rdi

  # Push the stack pointer (in second argument)
  push %rsi

  # Push flags
  pushf

  # Push code selector (in third argument)
  push %rdx

  # Push instruction pointer (in fourth argument)
  push %rcx

  # Shift remaining arguments to pass on to userspace (stack offsets account for pushes above)
  mov %r8, %rdi
  mov %r9, %rsi
  mov 48(%rsp), %rdx
  mov 56(%rsp), %rcx
  mov 64(%rsp), %r8
  mov 72(%rsp), %r9

  # Use iret to jump away
  iretq
