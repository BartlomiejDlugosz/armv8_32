ldr w21 gpio21
ldr w22 setup_gpio
str w21 [x22]
ldr w18 clear_gpio
ldr w19 set_gpio
ldr w20 bit21

ldr w17 zero
ldr w28 zero
ldr w29 zero

set:
str wzr [x18]
str w20 [x19]

delay:
    add x28, x28, #10000
delay_outer_loop:
    add x29, x29, #1000000
delay_inner_loop:
    subs x29, x29, #1
    b.ne delay_inner_loop
subs x28, x28, #1
b.ne delay_outer_loop

mov x17 x18
mov x18 x19
mov x19 x17

b set

setup_gpio:
 .int 0x3f200008
clear_gpio:
 .int 0x3f200028
set_gpio:
 .int 0x3f20001c
gpio21:
  .int 0x8
bit21:
  .int 0x200000
zero:
  .int 0x0
