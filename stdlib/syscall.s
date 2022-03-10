.global syscall

# This function is called to issue a system call
# Arguments are:
#  syscall number (in %rdi)
#  syscall arg0 (in %rsi)
#  syscall arg1 (in %rdx)
#  syscall arg2 (in %rcx)
#  syscall arg3 (in %r8)
#  syscall arg4 (in %r9)
#  syscall arg5 (at 0x8(%rsp))
syscall:
  # Pull argument 5 up into %rax
  mov 0x8(%rsp), %rax

  # Trigger the system call interrupt
  int $0x80

  # Return from the function
  retq
