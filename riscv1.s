.globl __start

.text

__start:
  addi  x4,x4,-1000
  lui  x5,0x80000
label:
  and  x6,x4,x5
  beq  x6,x0,out
  addi x10,x10,1
  slli  x4,x4,1
  jal  x11, label
out:
  sw   x10,0(x0)
  lw   x12,0(x0)

