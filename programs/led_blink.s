ldr w21 gpio21
ldr w22 setup_gpio
str w21 [x22] 

high:
ldr w20 bit21
str w0 [x20]
ldr wzr clear_gpio
ldr w0 set_gpio

delay:
add x28, x28, #1000
delay_outer_loop:
    add x29, x29, #1000000
delay_inner_loop:
    subs x29, x29, #1
    b.ne delay_inner_loop
subs x28, x28, #1
b.ne delay_outer_loop

low:
ldr w20 bit21
str w0 [x20]
ldr wzr set_gpio
ldr w0 clear_gpio

delay:
add x28, x28, #1000
delay_outer_loop:
    add x29, x29, #1000000
delay_inner_loop:
    subs x29, x29, #1
    b.ne delay_inner_loop
subs x28, x28, #1
b.ne delay_outer_loop

and x0, x0, x0
b high

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
