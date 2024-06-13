ldr w21 gpio21
str w21 #0x3f200008 

high:
str w0 bit21
ldr wzr #0x3f200028
ldr w0 #0x3f20001c

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
str w0 bit21
ldr wzr #0x3f20001c
ldr w0 #0x3f200028

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

gpio21:
  .int 0x8
bit21:
  .int 0x200000
